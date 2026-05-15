#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <tuple>
#include <SFML/Graphics.hpp>

float PI = 3.14159265358979;

std::vector<std::tuple<float, float, float>> rays_angles;

int screenx = sf::VideoMode::getDesktopMode().width;
int screeny = sf::VideoMode::getDesktopMode().height;

sf::View camera(sf::Vector2f(0, 0), sf::Vector2f(4000, 4000));

sf::Vector2f pos; //= sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

sf::Vector2f velocity;

//Player
sf::CircleShape shape(10);

struct Wall
{
    sf::Vector2f b_1;
    sf::Vector2f b_2;
};

struct Ray
{
    sf::RectangleShape line;

    sf::Vector2f s_pos = shape.getPosition();
    sf::Vector2f e_pos;

    float dx;
    float dy;

    float angle;
    float degrees;

    std::tuple<float, float, float> data;

    int set_rot()
    {
        dx = s_pos.x - e_pos.x;
        dy = s_pos.y - e_pos.y;

        angle = atan2(dy, dx);
        degrees = angle * 180 / PI;

        line.setRotation(degrees);

        return EXIT_SUCCESS;
    }

    int send_data_to_tuple()
    {
        std::get<0>(data) = angle;
        std::get<1>(data) = e_pos.x;
        std::get<2>(data) = e_pos.y;

        rays_angles.push_back(data);

        return EXIT_SUCCESS;
    };

    int cast_wall(Wall wall)
    {
        // ray x
        float x1 = s_pos.x;
        float x2 = e_pos.x;

        // wall x
        float x3 = wall.b_1.x;
        float x4 = wall.b_2.x;

        // ray y
        float y1 = s_pos.y;
        float y2 = e_pos.y;

        // wall y
        float y3 = wall.b_1.y;
        float y4 = wall.b_2.y;

        const float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        if (den == 0.0)
        {
            return 0;
        }
        else
        {
            const float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
            const float u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / den;

            if (0 < t && t < 1 && 0 < u && u < 1)
            {
                sf::Vector2f point;
                point.x = x1 + t * (x2 - x1);
                point.y = y1 + t * (y2 - y1);

                e_pos.x = point.x;
                e_pos.y = point.y;

                // std::cout<< e_pos.x <<std::endl;
                // std::cout<< e_pos.y <<std::endl;
                // std::cout<< "true" <<std::endl;
            }
        }
        return EXIT_SUCCESS;
    }
};

std::vector<Ray> rays;
std::vector<Wall> walls_vec;

int main()
{
    sf::RenderWindow window(sf::VideoMode(screenx, screeny), "SFML works!", sf::Style::Default);
    
    //Player
    shape.setPosition(1000, 540);
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(10, 10);

    bool can_move_up;
    bool can_move_down;
    bool can_move_right;
    bool can_move_left;
    
    sf::CircleShape circle;
    circle.setPosition(2000, 500);
    circle.setFillColor(sf::Color::Yellow);
    circle.setRadius(50);

    while (window.isOpen())
    {
        // Mouse position
        sf::Vector2f m_pos = sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

        // view
        camera.setCenter(shape.getPosition());

        // Creations

        //colision rays
        Ray colider1;
        Ray colider2;
        Ray colider3;
        Ray colider4;
        
        float colision_length = 20;

        colider1.e_pos = sf::Vector2f(shape.getPosition().x + colision_length, shape.getPosition().y);
        colider2.e_pos = sf::Vector2f(shape.getPosition().x - colision_length, shape.getPosition().y);
        colider3.e_pos = sf::Vector2f(shape.getPosition().x, shape.getPosition().y - colision_length);
        colider4.e_pos = sf::Vector2f(shape.getPosition().x, shape.getPosition().y + colision_length);

        // walls
        walls_vec.clear();

        Wall border_top_right;

        border_top_right.b_1 = sf::Vector2f(screenx * 2, 0);
        border_top_right.b_2 = sf::Vector2f(0, 0);

        Wall border_top_left;

        border_top_left.b_1 = sf::Vector2f(0, 0);
        border_top_left.b_2 = sf::Vector2f(0, screeny * 2);

        Wall border_buttom_left;

        border_buttom_left.b_1 = sf::Vector2f(0, screeny * 2);
        border_buttom_left.b_2 = sf::Vector2f(screenx * 2, screeny * 2);

        Wall border_buttom_right;

        border_buttom_right.b_1 = sf::Vector2f(screenx * 2, screeny * 2);
        border_buttom_right.b_2 = sf::Vector2f(screenx * 2, 0);

        Wall wall1;

        wall1.b_1 = sf::Vector2f(1800, 200);
        wall1.b_2 = sf::Vector2f(1800, 900);

        Wall wall2;

        wall2.b_1 = sf::Vector2f(1800, 900);
        wall2.b_2 = sf::Vector2f(2500, 900);

        Wall wall3;

        wall3.b_1 = sf::Vector2f(1200, 200);
        wall3.b_2 = sf::Vector2f(1200, 900);

        Wall wall4;

        wall4.b_1 = sf::Vector2f(2500, 900);
        wall4.b_2 = sf::Vector2f(3000, 900);

        Wall wall5;

        wall5.b_1 = sf::Vector2f(3300, 2000);
        wall5.b_2 = sf::Vector2f(2000, 2000);

        walls_vec.push_back(wall1);
        walls_vec.push_back(wall2);
        walls_vec.push_back(wall3);
        walls_vec.push_back(wall4);
        walls_vec.push_back(wall5);

        walls_vec.push_back(border_top_right);
        walls_vec.push_back(border_top_left);
        walls_vec.push_back(border_buttom_right);
        walls_vec.push_back(border_buttom_left);

        //World collision
        if (shape.getPosition().y <= 0)
        {
            can_move_up = false;
        }
        else
        {
            can_move_up = true;
        }
        if (shape.getPosition().y >= screeny * 2)
        {
            can_move_down = false;
        }
        else
        {
            can_move_down = true;
        }

        if (shape.getPosition().x >= screenx * 2)
        {
            can_move_right = false;
        }
        else
        {
            can_move_right = true;
        }
        if (shape.getPosition().x <= 0)
        {
            can_move_left = false;
        }
        else
        {
            can_move_left = true;
        }

        // rays
        rays.clear();

        float length = -100000.f;

        for (int i = 0; i < walls_vec.size(); i++)
        {
            Ray ray1;
            Ray ray2;
            Ray ray3;
            Ray ray4;
            Ray ray5;
            Ray ray6;

            ray1.e_pos = walls_vec[i].b_1;
            ray1.set_rot();

            ray2.angle = ray1.angle + 0.0001;
            ray2.e_pos.x = ray2.s_pos.x + length * cos(ray2.angle);
            ray2.e_pos.y = ray2.s_pos.y + length * sin(ray2.angle);

            ray3.angle = ray1.angle - 0.0001;
            ray3.e_pos.x = ray3.s_pos.x + length * cos(ray3.angle);
            ray3.e_pos.y = ray3.s_pos.y + length * sin(ray3.angle);

            ray4.e_pos = walls_vec[i].b_2;
            ray4.set_rot();

            ray5.angle = ray4.angle + 0.0001;
            ray5.e_pos.x = ray5.s_pos.x + length * cos(ray5.angle);
            ray5.e_pos.y = ray5.s_pos.y + length * sin(ray5.angle);

            ray6.angle = ray4.angle - 0.0001;
            ray6.e_pos.x = ray6.s_pos.x + length * cos(ray6.angle);
            ray6.e_pos.y = ray6.s_pos.y + length * sin(ray6.angle);

            rays.push_back(ray1);
            rays.push_back(ray2);
            rays.push_back(ray3);
            rays.push_back(ray4);
            rays.push_back(ray5);
            rays.push_back(ray6);
        }

        // rays casts

        rays_angles.clear();

        for (int i = 0; i < rays.size(); i++)
        {
            for (int j = 0; j < walls_vec.size(); j++)
            {
                rays[i].cast_wall(walls_vec[j]);
            }
        }
        
        for (int i = 0; i < rays.size(); i++)
        {
            rays[i].send_data_to_tuple();
        }

        sf::VertexArray walls(sf::Lines);

        for (int i = 0; i < walls_vec.size(); i++)
        {
            walls.append(sf::Vertex(sf::Vector2f(walls_vec[i].b_1)));
            walls.append(sf::Vertex(sf::Vector2f(walls_vec[i].b_2)));
            walls[i].color = sf::Color::Red;
        }

        std::sort(rays_angles.begin(), rays_angles.end());

        sf::VertexArray lines(sf::Lines);

        lines.clear();

        for (int i = 0; i < rays_angles.size(); i++)
        {
            lines.append(sf::Vertex(sf::Vector2f(shape.getPosition())));
            lines.append(sf::Vertex(sf::Vector2f(std::get<1>(rays_angles[i]), std::get<2>(rays_angles[i]))));
        }

        sf::VertexArray light(sf::TriangleFan, rays_angles.size() + 1);

        light[0].position = shape.getPosition();

        for (int i = 0; i < rays_angles.size(); i++)
        {
            light[i + 1].position = sf::Vector2f(std::get<1>(rays_angles[i]), std::get<2>(rays_angles[i]));
        }

        sf::VertexArray suplement_triagle(sf::Triangles, 3);

        suplement_triagle[0].position = shape.getPosition();
        suplement_triagle[1].position = sf::Vector2f(std::get<1>(rays_angles[0]), std::get<2>(rays_angles[0]));
        suplement_triagle[2].position = sf::Vector2f(std::get<1>(rays_angles[rays_angles.size() - 1]), std::get<2>(rays_angles[rays_angles.size() - 1]));

        for (int i = 0; i < 3; i++)
        {
            suplement_triagle[i].color = sf::Color(255, 255, 255, 1);
        }

        for (int i = 0; i < rays_angles.size() + 1; i++)
        {
            light[i].color = sf::Color(255, 255, 255, 1);
        }

        sf::RenderTexture rendert;
        rendert.create(screenx * 2, screeny * 2);

        sf::Sprite sprite;
        sprite.setTexture(rendert.getTexture(), true);
        sprite.setPosition(0, 0);

        //sf::Sprite sprite2;
        //sprite2.setTexture(rendert.getTexture(), true);
        //sprite2.setPosition(0, 0);

        /*
        sf::Texture back_tex;
        back_tex.loadFromFile("back.jpg");

        sf::Sprite s(back_tex);
        s.setPosition(0, 0);
        s.setScale(20, 12);
        */
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && can_move_up == true)
            {
                velocity.y = -10.0f;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && can_move_down == true)
            {
                velocity.y = 10.0f;
            }
            else
            {
                velocity.y = 0.0f;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && can_move_right == true)
            {
                velocity.x = 10.0f;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && can_move_left == true)
            {
                velocity.x = -10.0f;
            }
            else
            {
                velocity.x = 0.0f;
            }
        }

        shape.move(velocity);

        window.setView(camera);
        window.setFramerateLimit(120);

        rendert.clear();
        rendert.draw(light, sf::BlendNone);
        rendert.draw(suplement_triagle, sf::BlendNone);
        rendert.display();

        sprite.setTexture(rendert.getTexture(), true);
        //sprite2.setTexture(rendert.getTexture(), true);

        window.clear(sf::Color(100, 100, 100, 255));
        //window.draw(s);
        window.draw(circle);
        window.draw(sprite);
        //window.draw(sprite2);
        window.draw(shape);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            window.draw(walls);
            window.draw(lines);
        }
        window.display();
    }

    return 0;
}