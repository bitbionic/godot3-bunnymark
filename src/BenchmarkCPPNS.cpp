#include <vector>
#include <tuple>
#include <stdlib.h>
#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>
#include <Node2D.hpp>
#include <Texture.hpp>
#include <Sprite.hpp>
#include <Label.hpp>
#include <ResourceLoader.hpp>
#include <String.hpp>
#include <Engine.hpp>
#include <string> 
#include <stdio.h>

using namespace godot;

class BenchmarkCPPNS : public GodotScript<Node2D> {
        GODOT_CLASS(BenchmarkCPPNS);
public:
        Vector2 screenSize;
        std::vector<std::tuple<Vector2, Vector2>> bunnies;
        Ref<Texture> TBunny;
        float elapsed = 0.0f;
        float grav = 500; 

        BenchmarkCPPNS() { }

        void _ready() {
                srand (time(NULL));
                bunnies = {};
                screenSize = owner->get_viewport_rect().size;
                owner->get_node("../gui/list/add")->connect("pressed", owner, "add_bunnies");
                TBunny = ResourceLoader::load("res://bunny.png");
                for (int i = 0; i < 10; i++) {
                        add_bunny();
                }
                owner->set_process(true);
        }

        void _draw() {
                for (int i = 0; i < bunnies.size(); i++){
                        owner->draw_texture(Ref<Texture>(TBunny), std::get<0>(bunnies[i]), Color(1.,1.,1.,1.), Ref<Texture>());
                }
        }

        void _process(const float delta) {
                elapsed += delta;
                screenSize = owner->get_viewport_rect().size;
                if (elapsed > 1) {
                        String bunnies_count = std::to_string(Engine::get_frames_per_second()).c_str();
                        String label = "FPS: " + bunnies_count;
                        ((Label*)owner->get_node("../gui/list/fps"))->set_text(label); 
                }

                for (int i = 0; i < bunnies.size(); i++) {
                        std::tuple<Vector2, Vector2> bunny = bunnies[i];
                        Vector2 position = std::get<0>(bunny);
                        Vector2 newPosition = std::get<1>(bunny);

                        position.x += newPosition.x * delta;
                        position.y += newPosition.y * delta;

                        newPosition.y += grav * delta;

                        if (position.x > screenSize.x)
                        {
                            newPosition.x *= -1;
                            position.x = screenSize.x;
                        }
                        if (position.x < 0)
                        {
                            newPosition.x *= -1;
                            position.x = 0;
                        }

                        if (position.y > screenSize.y)
                        {
                            position.y = screenSize.y;
                            if ((double)rand() / RAND_MAX > 0.5)
                            {
                                newPosition.y = (rand() % 1100 + 50);
                            }
                            else
                            {
                                newPosition.y *= -0.85f;
                            }
                        }
            
                        if (position.y < 0)
                        {
                            newPosition.y = 0;
                            position.y = 0;
                        }
                        bunnies[i] = std::make_tuple(position, newPosition);
                }
                owner->update();
        }

        void add_bunnies() {
                for (int i = 0; i < 100; i++) {
                        add_bunny();
                }
        }

        void add_bunny() {
                bunnies.push_back(std::make_tuple(Vector2(screenSize.x / 2.0, screenSize.y / 2.0), Vector2(rand()%200+50, rand()%200+50)));
                String bunnies_count = std::to_string(bunnies.size()).c_str();
                String label = "bunny count: " + bunnies_count;
                ((Label*)owner->get_node("../gui/list/count"))->set_text(label);
        }


        static void _register_methods() {
                register_method((char *)"_ready", &BenchmarkCPPNS::_ready);
                register_method((char *)"_process", &BenchmarkCPPNS::_process);
                register_method((char *)"_draw", &BenchmarkCPPNS::_draw);
                register_method((char *)"add_bunnies", &BenchmarkCPPNS::add_bunnies);
        }
};

/** GDNative Initialize **/
GDNATIVE_INIT(godot_gdnative_init_options *options) {

}

/** GDNative Terminate **/
GDNATIVE_TERMINATE(godot_gdnative_terminate_options *options) {

}

/** NativeScript Initialize **/
NATIVESCRIPT_INIT() {
        register_class<BenchmarkCPPNS>();
}