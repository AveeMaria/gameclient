#include "../include/Game.hpp"



Game::Game()
{
    //comms = nullptr;
    //window = NULL;
    mouseX = 0;
    mouseY = 0;
}

Game::~Game()
{
    SDL_FreeSurface(icon);
}

void Game::networking(Comms* comms, UDPpacket* recvPacket)
{
    for (auto& t : towerRequests) {
        comms->stack_send(t, gameID);
    }
    towerRequests.clear();

    for (auto& e : enemyRequests) {
        if (comms->stack_send(e, gameID)) {
            //std::cout << "enemy request sent\n";
        }
        else {
            std::cout << "[ERROR]: cant send enemy request.\n";
        }
        
    }
    enemyRequests.clear();

    if (comms->recieve(recvPacket))
    {
        //ce je dubu packet z game IDjem ga setta za game id
        if ((Uint8)recvPacket->data[0] == (int)PacketType::INIT_GAME) {
            gameID = recvPacket->data[1];
            std::cout << "[INFO]:Game ID: " << (int)gameID << "\n";
            return;
        }
        //ce je dubu normal paketek prever ce je paketek namenjen temu gameIDju
        /*
        if ((Uint8)recvPacket->data[1] != gameID) {
            return;
        }*/
        //printBytes(reinterpret_cast<char*>(recvPacket->data), recvPacket->len);

        auto processStart = std::chrono::high_resolution_clock::now();

        switch ((Uint8)recvPacket->data[0]) {
        case 0:
            // std::cout << "type: PING\n";
            break;
        case 5:
            // std::cout << "type: PONG\n";
            break;
        case (int)PacketType::SYN:
            // std::cout << "ERROR: type: SYN\n";
            break;
        case (int)PacketType::SYN_ACK:
            // std::cout << "type: SYN_ACK\n";
            
            //ta ne posle gameidja
            if (!comms->stack_send(ACK{ SDL_GetTicks() }, recvPacket->address)) {
                std::cerr << "[ERROR]: ACK not sent.\n";
            }
            break;
        case (int)PacketType::ACK:
            // std::cout << "ERROR: type: ACK\n";
            break;
        case (int)PacketType::CREATE_TOWER:
            //std::cout << "recieved type: CREATE_TOWER\n";

            CreateTower ct;
            std::memcpy(&ct, &recvPacket->data[2], sizeof(CreateTower));
            towers.emplace_back(std::make_unique<Tower>(ct.id, ct.destRect, static_cast<TowerType>(ct.type)));

            if (defender) {
                //si defender
                myMoney -= Tower::getPrice(ct.type);
            }
            else {
                //si attacker
                enemyMoney -= Tower::getPrice(ct.type);
            }
        break;
        case (int)PacketType::CREATE_ENEMY:  
           CreateEnemy ce;  
           std::memcpy(&ce, &recvPacket->data[2], sizeof(CreateEnemy));  

           printBytes(reinterpret_cast<char*>(recvPacket->data), recvPacket->len);

           enemies.emplace_back(std::make_unique<Enemy>(ce.id, ce.destRect, static_cast<EnemyType>(ce.type)));
           
           if (defender) {
               //ce si defender
               enemyMoney -= Enemy::getPrice(ce.type + 1);
           }
           else {
               //attacker si
               myMoney -= Enemy::getPrice(ce.type + 1);
           }
        break;
        case (int)PacketType::DELETE_ENTITY:
        {
            int _id;
            std::memcpy(&_id, &recvPacket->data[2], sizeof(int));
            deletedEntityIDs.emplace_back(_id);
        }
        break;
        case (int)PacketType::INIT_TIMER:
        {
            InitTimer tdata;
            std::memcpy(&tdata, &recvPacket->data[2], sizeof(InitTimer));
            timer = std::make_unique<Timer>((uint32_t)tdata.time);
        }
        break;
        case (int)PacketType::ROLE:
        {
            bool r;
            std::memcpy(&r, &recvPacket->data[2], sizeof(r));
            defender = r;
            r ? std::cout << "[INFO]: ROLE: DEFENDER\n" : std::cout << "ROLE: ATTACKER\n";
        }
        case (int)PacketType::MONEY_INIT:
			MoneyInit mdata;
            std::memcpy(&mdata, &recvPacket->data[2], sizeof(MoneyInit));
            myMoney = mdata.money;
            enemyMoney = mdata.money;
        break;
        case (int)PacketType::TERMINATE:
			std::cout << "\n\n[WARNING]: TERMINATE GAME\n\n";
			isRunning = false;
            break;
        default:
            printBytes(reinterpret_cast<char*>(recvPacket->data), recvPacket->len);
            std::cout << "[WARNING]: Unknown packet type.\n";
            break;
        };

        auto processEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> processingTime = processEnd - processStart;
        //std::cout << "Packet processing time: " << processingTime.count() << " microseconds\n";
    }
}

void Game::init(const char* title, int width, int height, Uint8 _gameID)
{
    gameID = _gameID;
    int flags = 0;
    
    isRunning = true;

    icon = TextureManager::LoadSurface("../../../assets/icon.png");

    if (icon == NULL) {
        std::cerr << "ERROR: Failed to load icon. SDL_image: " << IMG_GetError() << "\n";
    }
    else {
        SDL_SetWindowIcon(Renderer::window, icon);
    }
    SDL_ShowCursor(SDL_DISABLE);

    textRenderer = std::make_unique<TextRenderer>();
    textRenderer->loadFont("../../../assets/fonts/MedievalSharp.ttf", 30);

    map = std::make_unique<Map>();
    cursor = std::make_unique<Cursor>("../../../assets/cursor.png");

	std::cout << "Game initialized\n";
}

void Game::handleEvents() {
    SDL_GetMouseState(&mouseX, &mouseY);
    mouse_coords = { mouseX, mouseY };

	cursor->setPos(mouseX, mouseY);
    
	//debuncni casovnik
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastMoveTime <= debounceDelay) {
        return;
    }
    lastMoveTime = currentTime;

    //X gumbek pa miska
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouse_down = true;

                Uint32 clickTime = SDL_GetTicks();                

                //ce je double click
                if (clickTime - lastclick <= DOUBLE_CLICK_DELAY) {
                    //std::cout << "Double click detected\n";

					//ce bos 2x kliknu na X se zapre shop modal
                    if (shop_modal != nullptr) {

                        switch (shop_modal->getSelectedOption(mouse_coords))
                        {
                        case -1:
                            shop_modal.reset();
                            break;
						case 1:
							if (defender) {
								std::cout << "spawn archer\n";
                                entity_place.setEntity(std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/archer.png")), 1);
							}
							else {
                                std::cout << "spawn goblin\n";
                                entity_place.setEntity(std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/goblin.png")), 1);
							}
                            shop_modal.reset();
							break;
                        case 2:
                            //ce je defender
                            if (defender) {
                                std::cout << "spawn mage\n";
                                entity_place.setEntity(std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/mage.png")), 2);
                            }
                            else {
                                std::cout << "spawn thief\n";
                                entity_place.setEntity(std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/thief.png")), 2);
                            }
                            shop_modal.reset();
                            break;
                        case 3:
                            if (defender) {
                                std::cout << "spawn barracks\n";
                                entity_place.setEntity(std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/barracks.png")), 3);
                            }
                            else {
                                std::cout << "spawn bandit\n";
                                entity_place.setEntity(std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/bandit.png")), 3);
                            }
                            shop_modal.reset();
                            break;
                        case 4:
                            if (defender) {
                                std::cout << "spawn mortar\n";
                                entity_place.setEntity(std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/mortar.png")), 4);
                            }
                            else {
                                std::cout << "spawn knight\n";
                                entity_place.setEntity(std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/knight.png")), 4);
                            }
                            shop_modal.reset();
                            break;
                        default:
                            break;
                        }
                        
                    }
                }
                
                lastclick = clickTime;               
            }

            //za rightclick
            else if (event.button.button == SDL_BUTTON_RIGHT) {
                
                if (entity_place.isSet()) {
                    if (defender) {
                        if (map->getMapValue(mouse_coords) == 0) {
							towerRequests.emplace_back(TowerRequest{ entity_place.getType() - 1, mouse_coords });
                            entity_place.deleteTex();
                        }
                        else {
                            //NC NAREST NEMORS POSTAVT
                        }
                    }
                    else {
                        if (map->getMapValue(mouse_coords) == 1) {
                            std::cout << "requesting enemy\n";
                            enemyRequests.emplace_back(EnemyRequest{ entity_place.getType() });
                            entity_place.deleteTex();
                        }
                        else {
                            //NC NAREST NEMORS POSTAVT
                        }
                    }
                }
                else {
                    std::cout << "entity_place not set";
                }

                /*
                for (auto& t : towers) {
                    //ce right clickas na barracke premikas stricke
                    if (t->getType() == TowerType::BARRACKS && Utils::coordInRect(mouse_coords, t->getRect())) {
                        std::cout << "barracks selected\n";
                    }
                }*/
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouse_down = false;
            }
            break;
        default:
            break;
        }
    }

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
        isRunning = false;
    }

    if (currentKeyStates[SDL_SCANCODE_L]) {
        Coords c = Utils::getTileMiddle(Tile{ 1, 0 });
        c.y -= TILESIZE / 4;
        enemies.emplace_back(std::make_unique<Enemy>(c));
    }

    if (currentKeyStates[SDL_SCANCODE_S]) {
        //spawna shop modal
        //predn placas nemors odpert shopa
		if (!entity_place.isSet()) {
            if (defender) {
                shop_modal = std::make_unique<ShopModal>("Build a new tower?", defender, 5, 3);
            }
            else {
                shop_modal = std::make_unique<ShopModal>("Send new troops?", defender, 5, 3);
            }
		}
		else {
			std::cout << "shop modal already open\n";
		}
    }

    //izpise entity count
    if (currentKeyStates[SDL_SCANCODE_K]) {
        Entity::printEntCnt();
    }

    /*neka arhajicna koda za risanje
    if (mouse_down && map_editor_mode) {
        map->paintValue(mouseX, mouseY);
    }

    //MAP EDITING
    if (currentKeyStates[SDL_SCANCODE_1]) {
        map->value = 0;
    }
    if (currentKeyStates[SDL_SCANCODE_2]) {
        map->value = 1;
    }
    if (currentKeyStates[SDL_SCANCODE_3]) {
        map->value = 2;
    }
    if (currentKeyStates[SDL_SCANCODE_4]) {
        map->value = 3;
    }
    if (currentKeyStates[SDL_SCANCODE_M]) {
        map->printMap();//to rab bit savemap
    }
     //izpise ce si u map editor mode
    if (currentKeyStates[SDL_SCANCODE_P]) {
        map_editor_mode = !map_editor_mode;
        std::cout << "map editor toggled: " << map_editor_mode << "\n";
    }
    */
}

void Game::update() {
    cursor->Update();

    if (timer) {
        timer->updateTimer();
    }

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        std::unique_ptr<Enemy>& e = *it;
        if (!e->alive() ||
            (std::find(deletedEntityIDs.begin(), deletedEntityIDs.end(), e->getID()) != deletedEntityIDs.end())) {
            if (defender) {
                myMoney += Enemy::getPrice((int)e->getType() + 1) / 2;
                myScore += Enemy::getPrice((int)e->getType() + 1) * 2;
            }
            else {
                enemyMoney += Enemy::getPrice((int)e->getType() + 1) / 2;
                enemyScore += Enemy::getPrice((int)e->getType() + 1) * 2;
            }

            //std::cout << "deleted enemy";
            it = enemies.erase(it);
        }
        //ce je enemy koncau svojo pot vrne true in ga zbrise
        else if (e->Move(map)) {
            if (defender) {
                enemyMoney += Enemy::getPrice((int)e->getType() + 1) / 2;
                enemyScore += Enemy::getPrice((int)e->getType() + 1) * 2;
			}
			else {
				myMoney += Enemy::getPrice((int)e->getType() + 1) / 2;
				myScore += Enemy::getPrice((int)e->getType() + 1) * 2;
			}

            it = enemies.erase(it);
        }
        else {
            e->Update();
            ++it;
        }      
    }
    deletedEntityIDs.clear();

    for (auto& t : towers) {
        t->updateAllies();
        
        for (auto it = enemies.begin(); it != enemies.end();) {
            std::unique_ptr<Enemy>& e = *it;
            e->Update();
            ++it;

            std::unique_ptr<Projectile> p = std::make_unique<Projectile>(false);
            
            
            if (t->moveProjectiles(p)) {
                //uzame dmg glede na tower level in na projectile type
                // V ONLINE MODE ZAKOMENTIREJ
                //e->takeDamage(t->calcDmg(p->getType()));
            }

            //k je enemy v dosegu
            if (Utils::getDistance(t->getCoords(), e->getCoords()) <= t->getRange()) {

                t->setTargetCoords(Utils::rectMiddle(e->getRect()));
                //ce je konc cooldowna
                if (t->canShoot(SDL_GetTicks())) {
                    t->makeProjecitle();
                }

                SDL_RenderDrawLine(Renderer::renderer, Utils::rectMiddle(t->getRect()).x, Utils::rectMiddle(t->getRect()).y, Utils::rectMiddle(e->getRect()).x, Utils::rectMiddle(e->getRect()).y);

                break;//da strelja samo eneega enemyja na enkat
            }
        }
        if (enemies.size() == 0) {
            t->deleteProjectiles();
        }

        t->updateProjectiles();
        t->Update();
    }

}

void Game::render() {
	if (Renderer::renderer == nullptr) {
		std::cerr << "ERROR: renderer is null.\n";
		return;
	}

    //Renderer::renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderClear(Renderer::renderer);

    map->DrawMap();

	for (auto& e : enemies) {
		e->Render();
	}

    for (auto& t : towers) {
        t->Render();
    }

    if (shop_modal != nullptr) {
        shop_modal->Render();
    }

    cursor->Render();

	if (entity_place.isSet()) {
        entity_place.setDestRect(mouse_coords.x - TILESIZE * 0.5, mouse_coords.y - TILESIZE * 0.5, TILESIZE, TILESIZE);
		entity_place.Render();
	}

    if (textRenderer == nullptr) {
        std::cout << "ERROR: textRenderer is null.\n";
    }
    else {
        if (timer) {
            textRenderer->renderText(timer->getFancyTime(), timer->getRect(), Color{200, 200, 200});
        }

        if (shop_modal != nullptr) {
            textRenderer->renderText(shop_modal->getTitle(), shop_modal->getTitleRect());
			textRenderer->renderText("X", shop_modal->getExitRect(), Color{ 255, 0, 0 });
			for (int i = 0; i < 4; ++i) {
                textRenderer->renderText(shop_modal->getDescriptionsA()[i], shop_modal->getDescRectA(i + 1));
                textRenderer->renderText(shop_modal->getDescriptionsB()[i], shop_modal->getDescRectB(i + 1));
			}
        }

        if (defender) {
            //ce si defender
            textRenderer->renderText("attacker (enemy)", enemyNameRect, Color{ 0, 0, 0 });
            textRenderer->renderText("defender (you)", myNameRect, Color{ 0, 0, 0 });
        }
        else {
            //ce si attacker
            textRenderer->renderText("defender (enemy)", enemyNameRect, Color{ 0, 0, 0 });
            textRenderer->renderText("attacker (you)", myNameRect, Color{ 0, 0, 0 });
        }

        textRenderer->renderText("money : " + std::to_string(myMoney), myMoneyRect, Color{0, 0, 0});
        textRenderer->renderText("money : " + std::to_string(enemyMoney), enemyMoneyRect, Color{0, 0, 0});

        textRenderer->renderText(std::to_string(enemyScore) + "-" + std::to_string(myScore), scoreRect, Color{ 0, 0, 0 });
    }

    SDL_RenderPresent(Renderer::renderer);
    
    //BREZ TEGA SDL_Net ne dela pravilno
    //SDL_DestroyRenderer(Renderer::renderer);
}

void Game::clean() {
    std::cout << "game cleaned\n";
}
