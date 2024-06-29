#include <iostream>
#include <cmath>
using namespace std;

class Circle{
private:
    double radius;       //радиус
    double ference;      //длина окружности
    double area;         //площадь круга
public:
    // Конструктор
    Circle(double r){
        radius = r;
        ference = 2 * M_PI * r;
        area = M_PI * pow(r, 2);
    }

    void set_radius(double r){
        radius = r;
        ference = 2 * M_PI * r;
        area = M_PI * pow(r, 2);
    }

    void set_ference(double f){
        ference = f;
        radius = f / (2 * M_PI);
        area = M_PI * pow(radius, 2);
    }

    void set_area(double a){
        area = a;
        radius = sqrt(a / M_PI);
        ference = 2 * M_PI * radius;
    }

    double get_radius(){
        return radius;
    }

    double get_ference(){
        return ference;
    }

    double get_area(){
        return area;
    }
};


int main(){
    // Земля и верёвка
    double earth_radius = 6378.1f;
    Circle earth(earth_radius);
    double new_ference = earth.get_ference() + 1; 
    earth.set_ference(new_ference);
    double new_radius = earth.get_radius();
    double gap = new_radius - earth_radius;
    cout << "Zazor = " << gap << " km" << endl;

    // Бассейн
    double dorozhka_width = 1.0f;
    double pool_radius = 3.0f;
    Circle pool(pool_radius + dorozhka_width);
    double fence_length = pool.get_ference();
    double fence_cost = 2000 * fence_length; 
    double dorozhka_area = pool.get_area() - M_PI * pow(pool_radius, 2);
    double dorozhka_cost = 1000 * dorozhka_area; 
    double total_cost = fence_cost + dorozhka_cost;
    cout << "Stoimost dorozhki = " << dorozhka_cost << " rub" << endl;
    cout << "Stoimost ogradi = " << fence_cost << " rub" << endl;
    cout << "Stoimost vsego = " << total_cost << " rub" << endl;

    return 0;
}
