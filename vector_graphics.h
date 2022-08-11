const float PI = 3.14159;

struct V2 { float x, y; V2(float X, float Y) { x=X; y=Y; } };

class VectorGraphic
{

public:

    //float PI = 3.1415;

    float x = 0.0;
    float y = 0.0;
    float rotation = 0.0;
    std::vector<V2> points;



    int size() { return points.size(); }

    void set_rotation(float rot) { rotation = rot; }
    void set_position(float X, float Y) { x=X; y=Y; }

    void add_point() { points.push_back(V2(x, y)); }
    void del_point(int i) { points.erase(points.begin()+i);}
    void insert_point(float X, float Y, int i) { V2 p = V2(X, Y); points.insert(points.begin()+i, p); }
    void set_point(float X, float Y, int i) { points[i].x = X; points[i].y = Y; }

    float distance(V2 v) { float X = v.x, Y = v.y; return sqrt((X*X) + (Y*Y)); }
    float distance_from(V2 v1, V2 v2) { float X = v1.x-v2.x, Y = v1.y-v2.y; return sqrt((X*X) + (Y*Y)); }

    int get_closest_point(float X, float Y)
    {
        float ld = sqrt((X*X) + (Y*Y));
        int closest_index = size(), index = 0;
        for (int i = 0; i < size(); i++) { float dx = points[i].x + x - X, dy = points[i].y + y - Y, d = sqrt((dx*dx) + (dy*dy)); if (d < ld) { ld = d; closest_index = i; } }
        return closest_index;
    }

    bool intersecting(float X, float Y, float limit=1.0)
    {
        for (int i = 0; i < size(); i++)
        {
            V2 check = V2(X-x, Y-y);
            float d = distance_from(check, points[i]);
        }

        return false;
    }

    void update_polygon(float scale=1.0)
    {
        int total = 0.0;
        int v = size();
        for (int i = 0; i < v; i++)
        {
            float r = distance(points[i])*scale;
            float I = float(float(i)/v);
            float nv = float( (rotation) + (I*(PI*2)) );
            points[i].x = r*cos(nv);
            points[i].y = r*sin(nv);
        }
    }

    void generate_polygon(float r, int v, float rot)
    {
        rotation = rot;
        points.clear();
        //for (int p = 0; p < v; p++) { points.push_back(V2()); }
        for (int i = 0; i < v; i++)
        {
            float I = float(float(i)/v);
            float nv = float( (rotation) + (I*(PI*2)) );
            points.push_back(V2(r*cos(nv), r*sin(nv)));
            //points[i].x = r*cos(nv);
            //points[i].y = r*sin(nv);
        }
    }

};
