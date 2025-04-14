#include "engine.h"

//make const colors
const color WHITE(1, 1, 1);
const color BLACK(0, 0, 0);
const color BLUE(0, 0, 1);
const color YELLOW(1, 1, 0);
const color BUBBLE(0.7, 0.8, 0.8);
const color BROWN(.537, .318, .161);
const color GREEN(0, 1, 0);
const color MARS(0.68, 0.38, 0.26);
const color RED(1, 0, 0);
const color ORANGE(1, .647, 0);

//int points set to 0
int points = 0;

Engine::Engine() {
    this->initWindow();
    this->initShaders();
    this->initShapes();
}

Engine::~Engine() {}


unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(WIDTH, HEIGHT, "engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    // Set uniforms
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes() {
    //initialize user
    user = make_unique<Rect>(shapeShader, vec2(0, 0), vec2(50, 35), WHITE);
    user1 = make_unique<Circle>(shapeShader, vec2(0, 0), 2, BLUE);
    user2 = make_unique<Circle>(shapeShader, vec2(0, 0), 2, RED);
    user3 = make_unique<Circle>(shapeShader, vec2(0, 0), 2, ORANGE);
    user4 = make_unique<Circle>(shapeShader, vec2(0, 0), 2, ORANGE);

    // Initialize meteor off-screen
    for (int i = 0; i < 5; ++i) {
        meteor.push_back(make_unique<Circle>(shapeShader, vec2(WIDTH + (rand() % WIDTH ) + 1000, rand() % HEIGHT),
                                           (rand() % 5 / 5.0) + 4, BROWN));
    }

    //initialize mars off-screen
    mars.push_back(make_unique<Circle>(shapeShader, vec2(200 + WIDTH, 300), 4, MARS));
    mars.push_back(make_unique<Circle>(shapeShader, vec2(190 + WIDTH, 310), 2.75, BROWN));
    mars.push_back(make_unique<Circle>(shapeShader, vec2(210 + WIDTH, 285), 2.5, BROWN));
    mars.push_back(make_unique<Circle>(shapeShader, vec2(218 + WIDTH, 310), 2.15, BROWN));

    //initialize earths
    earth.push_back(make_unique<Circle>(shapeShader, vec2(400, 300), 4, BLUE));
    earth.push_back(make_unique<Circle>(shapeShader, vec2(390, 310), 2.75, GREEN));
    earth.push_back(make_unique<Circle>(shapeShader, vec2(410, 290), 2.50, GREEN));
    earth.push_back(make_unique<Circle>(shapeShader, vec2(415, 280), 2.00, GREEN));
    earth.push_back(make_unique<Circle>(shapeShader, vec2(400, 320), 2.00, GREEN));


    //Initialize stars off-screen
    for (int i = 0; i < 75; ++i) {
        star.push_back(make_unique<Circle>(shapeShader, vec2(10 + (rand() % WIDTH), rand() % HEIGHT),
                                           (rand() % 3 / 5.0) + 1, WHITE));
    }

    //initialize score board
    score = make_unique<Rect>(shapeShader, vec2(WIDTH/2, HEIGHT), vec2(100, 50), WHITE);
}

void Engine::processInput() {
    glfwPollEvents();

    // Close window if escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Mouse position saved to check for collisions
    glfwGetCursorPos(window, &mouseX, &mouseY);
    mouseY = HEIGHT - mouseY; // make sure mouse y-axis isn't flipped

    //moves user objects with mouse
    user->setPos(vec2(mouseX, mouseY));
    user1->setPos(vec2(mouseX + 5, mouseY));
    user2->setPos(vec2(mouseX - 41, mouseY));
    user3->setPos(vec2(mouseX - 31, mouseY - 7));
    user4->setPos(vec2(mouseX - 31, mouseY + 7));

    //user clamp
    user->setPosX(
        std::clamp(user->getPos().x, user->getSize().x, (float)WIDTH - user->getSize().x)
    );
    user->setPosY(
        std::clamp(user->getPos().y, user->getSize().y, (float)HEIGHT - user->getSize().y)
    );

    //blue user1 clamp
    user1->setPosX(
        std::clamp(user1->getPos().x, user->getSize().x, (float)WIDTH - user->getSize().x + 5)
    );
    user1->setPosY(
        std::clamp(user1->getPos().y, user->getSize().y, (float)HEIGHT - user->getSize().y)
    );

    //flame clamps
    //red circle clamps
    user2->setPosX(
        std::clamp(user2->getPos().x, user->getSize().x - 41, (float)WIDTH - user->getSize().x - 38 - user2->getSize().x)
    );
    user2->setPosY(
        std::clamp(user2->getPos().y, user->getSize().y, (float)HEIGHT - user->getSize().y)
    );
    //orange circle clamps
    user3->setPosX(
        std::clamp(user3->getPos().x, user->getSize().x - 31, (float)WIDTH - user->getSize().x - 27 - user3->getSize().x)
    );
    user3->setPosY(
        std::clamp(user3->getPos().y, user->getSize().y - 7, (float)HEIGHT - user->getSize().y - 7)
    );
    //orange circle clamps
    user4->setPosX(
        std::clamp(user4->getPos().x, user->getSize().x - 31, (float)WIDTH - user->getSize().x - 27 - user4->getSize().x)
    );
    user4->setPosY(
        std::clamp(user4->getPos().y, user->getSize().y + 7, (float)HEIGHT - user->getSize().y + 7)
    );

}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //move stars respawn if off-screen
    for (unique_ptr<Shape> &flake : star) {
       flake->moveX(-flake->getSize().x / 5.0);
       if (flake->getPosX() < 0) {
           flake->setPos(vec2(WIDTH + flake->getSize().x, rand() % HEIGHT));
       }
    }
    //move meteors, respawn them if off-screen and add 1 to points
    for (unique_ptr<Circle> &meteor : meteor) {
        //increase speed depending on number of points
        if (points <= 25) {
            meteor->moveX(-meteor->getSize().x / 5.0 );
        }
        if (points > 25 && points <= 50) {
            meteor->moveX(-(meteor->getSize().x / 5.0 + 0.5));
        }
        if (points > 50 && points <= 75) {
            meteor->moveX(-(meteor->getSize().x / 5.0 + 1.5));
        }
        if (points > 75 && points < 100) {
            meteor->moveX(-(meteor->getSize().x / 5.0 + 2));
        }
        if (points == 100) {
            meteor->moveX(-(meteor->getSize().x / 5.0 + 2.5));
        }
        //respawn meteors add a point
        if (meteor->getPosX() < -50 && points < 100) {
            meteor->setPos(vec2(WIDTH + meteor->getSize().x, rand() % HEIGHT));
            points = points + 1;
            //Debug code used to see current points number.
            //cout << points << endl;
        }
    }

    //move earth off-screen
    for (unique_ptr<Shape> &earth : earth) {
        earth->moveX(-1);
    }

    //if points > 100 move mars on screen
    for (unique_ptr<Shape> &mars : mars) {
        if (points > 99) {
            if (mars->getPosX() == 400 ) {
                mars->moveX(0);
            }
            else {
                mars->moveX(-1);
            }
        }
    }

    //detect collisions with user and meteors
    for (unique_ptr<Circle>& c : meteor) {
        if (c->isOverlapping(*user)) {
            glfwSetWindowShouldClose(window, true);
        }
    }
}

void Engine::render() {
    glClearColor(BLACK.red, BLACK.green, BLACK.blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set shader to use for all shapes
    shapeShader.use();

    //draw all stars
    for (auto &s : star) {
        s->setUniforms();
        s->draw();
    }

    //draw all meteors
    for (auto &s : meteor) {
        s->setUniforms();
        s->draw();
    }

    //draw earth shapes
    for (auto &s : earth) {
        s->setUniforms();
        s->draw();
    }

    //draw mars shapes
    for (auto &s : mars) {
        s->setUniforms();
        s->draw();
    }

    //draw user rocket
    user->setUniforms();
    user->draw();

    user1->setUniforms();
    user1->draw();

    user2->setUniforms();
    user2->draw();

    user3->setUniforms();
    user3->draw();

    user4->setUniforms();
    user4->draw();

    //score->setUniforms();
    //score->draw();
    glfwSwapBuffers(window);
}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}