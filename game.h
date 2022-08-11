#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "var.h"
#include"vector_graphics.h"

class Game : public olc::PixelGameEngine
{
public:
    Game()
    {
        sAppName = "Vector Graphics";
    }

public:

    bool debug = true;
    bool rotate_selected = true;
    bool mouse_distances = true;

    float rotation = 0.0;
    float radius = 32.0;
    float speed = 0.01;
    int vertices = 5;
    int selected_polygon = 0;

    olc::Pixel lines_color = olc::Pixel(32, 32, 32);
    olc::Pixel points_color = olc::Pixel(64, 64, 64);
    olc::Pixel position_color = olc::Pixel(128, 128, 128);
    olc::Pixel nearest_color = olc::Pixel(255, 255, 255);

    std::vector<VectorGraphic> polygons;



    //
    //
    //

    void start()
    {
        std::cout << "Hello, Vector Graphics!" << std::endl;
    }



    //
    //
    //
    
    void add_polygon()
    {
        polygons.push_back(VectorGraphic());
    }

    void del_polygon()
    {
        polygons.erase(polygons.begin()+selected_polygon);
    }



    //
    //
    //

    int get_nearest_vertex(float X, float Y)
    {
        float ld = sqrt((X*X) + (Y*Y));
        int closest_index = 0;
        int index = 0;

        int limit = polygons[selected_polygon].size();
        for (int i = 0; i < limit; i++)
        {
            float dx = polygons[selected_polygon].points[i].x + polygons[selected_polygon].x - X;
            float dy = polygons[selected_polygon].points[i].y + polygons[selected_polygon].y - Y;
            float d = sqrt((dx*dx) + (dy*dy));
            if (d < ld)
            {
                ld = d;
                closest_index = i;
            }
            if (debug)
            {
                DrawCircle(dx+X, dy+Y, d, olc::Pixel(0, i*(255/limit), 128));
                if (mouse_distances) { DrawCircle(GetMouseX(), GetMouseY(), d, olc::Pixel(i*(255/limit), 128, 0)); }
            }
        }
        return closest_index;
    }



    //
    //
    //

    void draw_info()
    {

        std::string a = std::to_string(selected_polygon+1);
        std::string b = std::to_string(polygons.size());
        std::string sel = a + "/" + b;
        std::string c = "";

        std::string rot = std::to_string(rotation);
        std::string rad = std::to_string(radius);
        std::string vrt = std::to_string(vertices);


        if (polygons.size() > 0)
        {
            c = std::to_string(polygons[selected_polygon].size());
        }

        DrawStringDecal({4, 4}, "Selected: "+sel, olc::YELLOW, {1.0f, 1.0f});
        DrawStringDecal({4, 12}, "Polygon Size: "+c, olc::YELLOW, {1.0f, 1.0f});
        DrawStringDecal({4, 20}, "Rotation: "+rot, olc::YELLOW, {1.0f, 1.0f});
        DrawStringDecal({4, 28}, "Radius: "+rad, olc::YELLOW, {1.0f, 1.0f});
        DrawStringDecal({4, 36}, "Verices: "+vrt, olc::YELLOW, {1.0f, 1.0f});
    }



    //
    //
    //

    void draw_polygon(int i)
    {
        float offx = polygons[i].x;
        float offy = polygons[i].y;
        
        if (polygons[i].size() > 0)
        {
            // Draw Lines of Polygon
            for (int p = 0; p < polygons[i].size(); p++)
            {
                float x = polygons[i].points[p].x+offx;
                float y = polygons[i].points[p].y+offy;
                if (p > 0)
                {
                    float x_ = polygons[i].points[p-1].x+offx;
                    float y_ = polygons[i].points[p-1].y+offy;
                    DrawLine(x_,y_, x,y, lines_color);
                }
            }
            int index = polygons[i].size()-1;
            float x = polygons[i].points[0].x+offx;
            float y = polygons[i].points[0].y+offy;
            float x_ = polygons[i].points[index].x+offx;
            float y_ = polygons[i].points[index].y+offy;
            DrawLine(x_,y_, x,y, lines_color);
            // Draw Points of Polygon
            for (int p = 0; p < polygons[i].size(); p++)
            {
                float x = polygons[i].points[p].x+offx;
                float y = polygons[i].points[p].y+offy;
                Draw(x, y, points_color);
                if (p == get_nearest_vertex(GetMouseX(), GetMouseY()) && i == selected_polygon) { Draw(x, y, nearest_color); }
            }
        }
        // Draw Position of Polygon
        Draw(offx, offy, position_color);
    }



    //
    //
    //

    void update_rotations()
    {
        rotation += speed;
        if (rotation > 6.28) { rotation -= 6.28; }
    }

    bool OnUserCreate() override
    {
        start();
        return true;
    }

    void construct_polygon()
    {
        add_polygon();
        selected_polygon = polygons.size()-1;
        polygons[selected_polygon].x = GetMouseX();
        polygons[selected_polygon].y = GetMouseY();
        int count = 0;
        for (int y = -(vertices-1); y < vertices; y++)
        {
            for (int x = -(vertices-1); x < vertices; x++)
            {
                float X = x*radius;
                float Y = y*radius;
                polygons[selected_polygon].insert_point(X, Y, count);
                count++;
            }
        }
    }

    bool collision(int c1, int c2)
    {
        for (int v = 0; v < polygons[c1].size(); v++)
        {
            float X = polygons[c1].points[v].x;
            float Y = polygons[c1].points[v].y;
            if (polygons[c2].intersecting(X, Y))
            {
                return true;
            }
        }
        return false;
    }

    bool key_state(int state)
    {
        switch (state)
        { // 1-CTRL 2-SHIFT 3-TAB 0-NONE
            case 0 : { if (!GetKey(olc::Key::CTRL).bHeld && !GetKey(olc::Key::SHIFT).bHeld && !GetKey(olc::Key::TAB).bHeld) return true; } break;
            case 1 : { if (GetKey(olc::Key::CTRL).bHeld && !GetKey(olc::Key::SHIFT).bHeld && !GetKey(olc::Key::TAB).bHeld) return true; } break;
            case 2 : { if (GetKey(olc::Key::SHIFT).bHeld && !GetKey(olc::Key::CTRL).bHeld && !GetKey(olc::Key::TAB).bHeld) return true; } break;
            case 3 : { if (GetKey(olc::Key::TAB).bHeld && !GetKey(olc::Key::CTRL).bHeld && !GetKey(olc::Key::SHIFT).bHeld) return true; } break;
        }
        return false;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Stuff

        Clear(olc::BLACK);

        if (polygons.size() > 0)
        {
            if (polygons[0].intersecting(GetMouseX(), GetMouseY())) { std::cout << "Intersecting Edge" << std::endl; }
        }

        // (Click)
        if (key_state(0))
        {
            // Add Vertex
            if (GetMouse(0).bReleased)
            {
                if (polygons.size() > 0)
                {
                    float X = GetMouseX()-polygons[selected_polygon].x;
                    float Y = GetMouseY()-polygons[selected_polygon].y;
                    int trgt = polygons[selected_polygon].get_closest_point(GetMouseX(), GetMouseY());
                    polygons[selected_polygon].insert_point(X, Y, trgt);
                }
            }
            // Delete Vertex
            if (GetMouse(1).bReleased)
            {
                if (polygons.size() > 0)
                {
                    float X = GetMouseX()-polygons[selected_polygon].x;
                    float Y = GetMouseY()-polygons[selected_polygon].y;
                    int verts = polygons[selected_polygon].size();
                    int trgt = polygons[selected_polygon].get_closest_point(GetMouseX(), GetMouseY());
                    if (verts > 0)
                    {
                        polygons[selected_polygon].del_point(trgt);
                    }
                    else
                    {
                        del_polygon();
                        selected_polygon = polygons.size()-1;
                    }
                }
            }
        }
        // (Ctrl-Click)
        if (key_state(1) && polygons.size() > 0)
        {
            // Move Closest Vertex
            if (GetMouse(0).bHeld)
            {
                float X = GetMouseX()-polygons[selected_polygon].x;
                float Y = GetMouseY()-polygons[selected_polygon].y;
                int trgt = polygons[selected_polygon].get_closest_point(GetMouseX(), GetMouseY());
                polygons[selected_polygon].set_point(X, Y, trgt);
            }
            // Move Polygon
            if (GetMouse(1).bHeld)
            {
                float X = GetMouseX();
                float Y = GetMouseY();
                polygons[selected_polygon].set_position(X, Y);
            }
        }
        // (Shift-Click)
        if (key_state(2))
        {
            // Add Generated Polygon
            if (GetMouse(0).bReleased)
            {
                add_polygon();
                selected_polygon = polygons.size()-1;
                polygons[selected_polygon].set_position(GetMouseX(), GetMouseY());
                polygons[selected_polygon].generate_polygon(radius, vertices, rotation);
            }
            // Remove Polygon
            if (GetMouse(1).bReleased)
            {
                if (selected_polygon > 0)
                {
                    selected_polygon = polygons.size()-1;
                    del_polygon();
                }
            }
        }
        // Add Custom Polygon (Tab-Click)
        if (key_state(3))
        {
            if (GetMouse(0).bReleased)
            {
                add_polygon();
                selected_polygon = polygons.size()-1;
                polygons[selected_polygon].set_position(GetMouseX(), GetMouseY());
            }
        }

        // Scale Polygon
        if (GetKey(olc::Key::Q).bPressed) { polygons[selected_polygon].update_polygon(1.1); }
        if (GetKey(olc::Key::E).bPressed) { polygons[selected_polygon].update_polygon(0.9); }
        // Selected Polygon
        if (GetKey(olc::Key::Z).bPressed) { if (selected_polygon > 0) selected_polygon--; }
        if (GetKey(olc::Key::C).bPressed) { if (selected_polygon < polygons.size()-1) selected_polygon++; }
        // Vertex Count
        if (GetKey(olc::Key::W).bPressed) { vertices++; }
        if (GetKey(olc::Key::S).bPressed) { if (vertices > 1) vertices--; }
        // Radius Scale
        if (GetKey(olc::Key::A).bHeld) { radius -= 0.1; }
        if (GetKey(olc::Key::D).bHeld) { radius += 0.1; }
        // Toggle Debug
        if (GetKey(olc::Key::T).bPressed) { debug = !debug; }
        if (GetKey(olc::Key::R).bPressed) { rotate_selected = !rotate_selected; }
        if (GetKey(olc::Key::M).bPressed) { mouse_distances = !mouse_distances; }
        if (GetKey(olc::Key::L).bPressed) { construct_polygon(); }

        update_rotations();
        // Update Polygon Shape
        for (int i = 0; i < polygons.size(); i++)
        {
            if (!rotate_selected)
            {
                if (i != selected_polygon) polygons[i].update_polygon();
            }
            else { polygons[i].update_polygon(); }
            polygons[i].rotation = rotation;
            draw_polygon(i);
        }

        draw_info();
        // End Frame
        return true;
    }
};
