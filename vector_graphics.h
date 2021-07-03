struct V2
{
    float x;
    float y;
};

class VectorGraphic
{

public:

    float PI = 3.1415;

    float x = 0.0;
    float y = 0.0;
    float rotation = 0.0;
    std::vector<V2> points;



    int size() { return points.size(); }

    void set_rotation(float rot) { rotation = rot; }
    void set_position(float X, float Y)
    {
        x = X;
        y = Y;
    }

    void add_point() { points.push_back(V2()); }
    
    void del_point(int i) { points.erase(points.begin()+i);}
    
    void insert_point(float X, float Y, int i)
    {
        V2 p = V2();
        p.x = X;
        p.y = Y;
        points.insert(points.begin()+i, p);
    }
    
    void set_point(float X, float Y, int i)
    {
        points[i].x = X;
        points[i].y = Y;
    }

    float distance(V2 v)
    {
        // (v.x-x) + (v.y - y)
        float X = v.x;
        float Y = v.y;
        float d = sqrt((X*X) + (Y*Y));
        return d;
    }

    int get_closest_point(float X, float Y)
    {
        float ld = sqrt((X*X) + (Y*Y));
        int closest_index = size();
        int index = 0;

        for (int i = 0; i < size(); i++)
        {
            float dx = points[i].x + x - X;
            float dy = points[i].y + y - Y;
            float d = sqrt((dx*dx) + (dy*dy));
            if (d < ld)
            {
                ld = d;
                closest_index = i;
            }
        }
        return closest_index;
    }

    void update_polygon(float scale=1.0)
    {
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
        for (int p = 0; p < v; p++) { points.push_back(V2()); }
        for (int i = 0; i < v; i++)
        {
            float I = float(float(i)/v);
            float nv = float( (rotation) + (I*(PI*2)) );
            points[i].x = r*cos(nv);
            points[i].y = r*sin(nv);
        }
    }

};
