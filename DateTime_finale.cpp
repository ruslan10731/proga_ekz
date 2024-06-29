#include <iostream>
#include <ctime>
using namespace std;

class DateTime{
private:
    struct tm dateInfo;
    string to_string(tm& other){
        char buffer[80];
        strftime(buffer, 80, "%d %B %Y, %A", &other);
        return string(buffer);
    }

public:
    // Конструктор с тремя числовыми параметрами (день, месяц, год)
    DateTime(int day, int month, int year){
        dateInfo = tm();
        dateInfo.tm_year = year - 1900;
        dateInfo.tm_mon = month - 1;
        dateInfo.tm_mday = day;
    }

    // Конструктор из строки (в формате "22 january 2024, monday")
    DateTime(const string& dateStr){
        dateInfo = tm();
        strptime(dateStr.c_str(), "%d %B %Y, %A", &dateInfo);
    }

    // Конструктор без параметров (объект использует текущую дату)
    DateTime(){
        time_t now = time(0);
        dateInfo = *localtime(&now);
    }

    // Конструктор копирования (создаём копию другого объекта)
    DateTime(const DateTime& other){
        dateInfo = other.dateInfo;
    }


    /*Возвращение текущей даты в виде строки, с указанием дня 
    недели и названия месяца (например 07 november 2018, wednesday)*/
   string get_today(){
        return to_string(dateInfo);
    }

    // Возвращение даты вчерашнего дня в виде строки
    string get_yesterday(){
        time_t yesterday = mktime(&dateInfo) - 86400;
        tm result = *localtime(&yesterday);
        return to_string(result);
    }

    // Возвращение даты завтрашнего дня в виде строки
    string get_tomorrow(){
        time_t tomorrow = mktime(&dateInfo) + 86400;
        tm result = *localtime(&tomorrow);
        return to_string(result);
    }

    // Возвращение даты через N дней в будущем
    string get_future(unsigned int N){
        time_t future = mktime(&dateInfo) + N * 86400;
        tm result = *localtime(&future);
        return to_string(result);
    }

    // Возвращение даты через N дней в прошлом
    string get_past(unsigned int N){
        time_t past = mktime(&dateInfo) - N * 86400;
        tm result = *localtime(&past);
        return to_string(result);
    }

    // Для расчёта разницы (в днях) между двумя датами
    int get_difference(DateTime& other){
        tm firstDate = tm();
        firstDate.tm_mday = dateInfo.tm_mday;
        firstDate.tm_mon = dateInfo.tm_mon;
        firstDate.tm_year = dateInfo.tm_year;
        tm secondDate = tm();
        secondDate.tm_mday = other.dateInfo.tm_mday;
        secondDate.tm_mon = other.dateInfo.tm_mon;
        secondDate.tm_year = other.dateInfo.tm_year;

        time_t t1 = mktime(&firstDate);
        time_t t2 = mktime(&secondDate);
        double seconds = 0;
        if (t1 > t2)
            seconds = difftime(t1, t2);
        else
            seconds = difftime(t2, t1);

        return seconds / 86400;
    }
};


int main(){
    DateTime today;
    cout << "Vchera - " << today.get_yesterday() << endl;
    cout << "Segodnya - " << today.get_today() << endl;
    cout << "Zavtra - " << today.get_tomorrow() << endl;
    cout << "Cherez 8 dney - " << today.get_future(8) << endl;
    cout << "21 dney nazad - " << today.get_past(21) << endl;
    DateTime futureDate = DateTime(today.get_past(21));
    cout << "Raznitsa mezhdu datami - " << today.get_difference(futureDate) << " dney" << endl;

    return 0;
}
