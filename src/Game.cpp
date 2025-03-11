#include "../include/Game.hpp"

int Entity::ent_cnt = 0;

std::unique_ptr<Timer> timer;

std::unique_ptr<TextRenderer> textRenderer;

std::unique_ptr<Map> map;

Coords mouse_coords = { 0, 0 };

std::vector<std::unique_ptr<Enemy>> enemies;

std::vector<std::unique_ptr<Tower>> towers;

std::unique_ptr<Modal> modal;

std::unique_ptr<Cursor> cursor;

Game::Game()
{
    

    //comms = Comms("127.0.0.1", (Uint16)123456);

    //comms = nullptr;
    //window = NULL;
    mouseX = 0;
    mouseY = 0;
}

Game::~Game()
{
    SDL_FreeSurface(icon);
}

void Game::networking(Comms* comms) {
    if (comms->stack_send(SYN{ (int)SDL_GetTicks() })) {
        std::cout << "SYN SENT\n";
    }

    /*
    UDPpacket* recvPacket;
    if (comms->recieve(&recvPacket)) {
        if (recvPacket->len == 0) {
            std::cout << "ERROR: EMPTY PACKET";
            //continue;
            return;
        }
        printBytes(reinterpret_cast<char*>(recvPacket->data), recvPacket->len);

        ///PREVER KER PACKET JE PO PRVEM BYTU

        switch ((Uint8)recvPacket->data[0]) {
        case 0:
            std::cout << "type: PING\n";
            break;
        case 5:
            std::cout << "type: PONG\n";
            break;
            ///////
        case (int)PacketType::SYN:
            std::cout << "ERROR: type: SYN\n";//TEGA CLIENT NE SPREJEMA KER POSLJE
            break;
        case (int)PacketType::SYN_ACK:
            std::cout << "type: SYN_ACK\n";

            if (!comms->stack_send(ACK{ SDL_GetTicks() }, recvPacket->address)) {
                std::cerr << "ERROR: ACK not sent.\n";
            }

            break;

        case (int)PacketType::ACK:
            std::cout << "ERROR: type: ACK\n";//TEGA CLIENT NE SPREJEMA KER POSLJE
            break;
            ///////
        case 40:
            std::cout << "type: ENTITY_POS\n";
            break;
        default:
            std::cout << "Unknown packet type.\n";
            break;
        }
    }
    */
    
}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
    int flags = 0;

    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    
   
        
    
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
    textRenderer->loadFont("../../../assets/fonts/MedievalSharp.ttf", 20);

    map = std::make_unique<Map>();
    cursor = std::make_unique<Cursor>("../../../assets/cursor.png");

    towers.emplace_back(std::make_unique<Tower>(TowerType::ARCHER, 1, 2));
    towers.emplace_back(std::make_unique<Tower>(TowerType::MAGE, 5, 1));
    towers.emplace_back(std::make_unique<Tower>(TowerType::BARRACKS, 3, 1));
    towers.emplace_back(std::make_unique<Tower>(TowerType::MORTAR, 3, 3));

    Coords c = Utils::getTileMiddle(Tile{ 1, 0 });
    c.y -= TILESIZE / 4;

    enemies.emplace_back(std::make_unique<Enemy>(c));

    //std::cout << "towers: " << towers.size() << "\n"; std::cout << "enemies: " << enemies.size() << "\n";

    timer = std::make_unique<Timer>((uint32_t)90);

    /*
    ///NETWORKING
    comms = new Comms("127.0.0.1", 123456);

    //TODO FIX ERROR
    if (comms->stack_send(SYN{ SDL_GetTicks() })) {
        std::cout << "SYN SENT\n";
    }*/


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
                    std::cout << "Double click detected\n";

                    //ce bos 2x kliknu na stolp se odpre modal z specsi/upgradi cene pa shop za postavt towerje
                    modal = std::make_unique<Modal>("Build new tower?", 5 * TILESIZE, 3 * TILESIZE);
                }
                lastclick = clickTime;               
            }

            //za rightclick
            else if (event.button.button == SDL_BUTTON_RIGHT) {
                for (auto& t : towers) {
                    //ce right clickas na barracke premikas stricke
                    if (t->getType() == TowerType::BARRACKS && Utils::coordInRect(mouse_coords, t->getRect())) {
                        std::cout << "barracks selected\n";
                    }
                }
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
    
    //neka arhajicna koda za risanje
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

    if (currentKeyStates[SDL_SCANCODE_L]) {
        Coords c = Utils::getTileMiddle(Tile{ 1, 0 });
        c.y -= TILESIZE / 4;
        enemies.emplace_back(std::make_unique<Enemy>(c));
    }

    //izpise ce si u map editor mode
    if (currentKeyStates[SDL_SCANCODE_P]) {
        map_editor_mode = !map_editor_mode;
        std::cout << "map editor toggled: " << map_editor_mode << "\n";
    }

    //izpise entity count
    if (currentKeyStates[SDL_SCANCODE_K]) {
        Entity::printEntCnt();
    }
}

void Game::update() {
    cursor->Update();

    if (timer->done()) {
        //std::cout << "timer finished\n";
    }
    else {
        timer->updateTimer();
    }

    for (auto& t : towers) {
        t->updateAllies();
        
        for (auto it = enemies.begin(); it != enemies.end();) {
            std::unique_ptr<Enemy>& e = *it;
            e->Update();
            ++it;

            std::unique_ptr<Projectile> p = std::make_unique<Projectile>(false);
            if (t->moveProjectiles(p)) {
                //uzame dmg glede na tower level in na projectile type
				e->takeDamage(t->calcDmg(p->getType()));
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

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        std::unique_ptr<Enemy>& e = *it;
        e->Move(map);
        e->Update();

        if (!e->alive()){
            it = enemies.erase(it); break;
        }
        else {
			++it;
        }
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

    if (modal != nullptr) {
        modal->Render();
    }

    cursor->Render();

    if (textRenderer == nullptr) {
        std::cout << "ERROR: textRenderer is null.\n";
    }
    else {
        SDL_Rect r = { 512, 0, 128, 64 };

        SDL_RenderDrawRect(Renderer::renderer, &r);
        textRenderer->renderText(timer->getFancyTime(), r, Color{ 200, 200, 200 });
        if (modal != nullptr) {
            textRenderer->renderText(modal->getTitle(), modal->getTitleRect());
        }
    }

    SDL_RenderPresent(Renderer::renderer);
    

    //BREZ TEGA SDL_Net ne dela pravilno
    //SDL_DestroyRenderer(Renderer::renderer);

}

void Game::clean() {
    
    std::cout << "game cleaned\n";
}