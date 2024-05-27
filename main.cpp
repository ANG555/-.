#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include"MyMenu.h"
#include<Windows.h>
#include<iostream>
#include <vector>
#include <fstream>
#include <map>
#include <ctime>
#include <iomanip>
using namespace sf;
using namespace std;

struct TextFormat
{
    int size_font = 60; // Размер шрифта
    Color menu_text_color = Color::White; // Цвет текста
    float bord = 0.0f; // Толщина обводки
    Color border_color = Color::Black; // Цвет обводки
};// Структура для хранения параметров форматирования текста

// Функция настройки текста
void InitText(Text& mtext, float xpos, float ypos, const String& str, TextFormat Ftext)
{
    // Установка параметров текста
    mtext.setCharacterSize(Ftext.size_font);
    mtext.setPosition(xpos, ypos);
    mtext.setString(str);
    mtext.setFillColor(Ftext.menu_text_color);
    mtext.setOutlineThickness(Ftext.bord);
    mtext.setOutlineColor(Ftext.border_color);
}

// Структура для представления элемента
struct Element {
    int type; // Тип элемента
    int count; // Количество элементов
    int not_type; // Тип элемента, который не должен быть в одной коробке с данным
};

// Функция для проверки, можно ли добавить элемент в коробку
bool canAddToBox(vector<int>& box, int type, int not_type) {
    for (int t : box) {
        if (t == not_type) {
            return false; // Найден элемент, который не должен быть вместе
        }
    }
    return true;
}

// Функция для расфасовки элементов по коробкам
void packElements(int z1, int z2, int z3, int z4, int z5, int zv1, int zv2, int zv3) {


    int boxCount10 = zv3, boxCount5 = zv2, boxCount3 = zv1;

    // Создаем вектор boxSizes
    vector<int> boxSizes(boxCount10 + boxCount5 + boxCount3);
    for (int i = 0; i < boxCount10; i++) {
        boxSizes[i] = 10;
    }
    for (int i = boxCount10; i < boxCount10 + boxCount5; i++) {
        boxSizes[i] = 5;
    }
    for (int i = boxCount10 + boxCount5; i < boxCount10 + boxCount5 + boxCount3; i++) {
        boxSizes[i] = 3;
    }

    vector<Element> elements = {
                {1, z1, 5},  // Тип 1, 4 элемента, не должен быть с типом 2
                {2, z2, 0},  // Тип 2, 3 элемента, не должен быть с типом 1
                {3, z3, 4},  // Тип 3, 2 элемента, не должен быть с типом 3
                {4, z4, 3},  // Тип 4, 5 элементов, не должен быть с типом 2
                {5, z5, 1},  // Тип 5, 1 элемент, не должен быть с типом 3
    };

    // Создаем коробки
    vector<vector<int>> boxes(boxSizes.size());

    // Используем map для подсчета не влезших элементов
    map<int, int> notFitElements;

    // Перебираем все элементы
    for (auto& elem : elements) {
        int type = elem.type;    // Тип текущего элемента
        int count = elem.count;  // Количество текущего элемента
        int not_type = elem.not_type; // Тип, который не должен быть в одной коробке

        // Расфасовываем элемент по коробкам
        for (int i = 0; i < count; i++) {
            bool placed = false;
            for (int j = 0; j < boxSizes.size(); j++) {
                if (boxes[j].size() < boxSizes[j] && canAddToBox(boxes[j], type, not_type)) {
                    boxes[j].push_back(type);
                    placed = true;
                    break;
                }
            }
            // Если нет места ни в одной коробке или нарушается условие, увеличиваем счетчик для данного типа
            if (!placed) {
                notFitElements[type]++;
            }
        }
    }

    // Записываем содержимое коробок в файл
    ofstream outFile("result.txt");

    // Получаем текущее время и дату
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    // Форматируем дату и время
    outFile << "===================================================================================" << endl;
    outFile << "Дата и время поставки: "
        << ltm.tm_mday << "/"
        << ltm.tm_mon + 1 << "/"
        << ltm.tm_year + 1900 << " "
        << setfill('0') << setw(2) << ltm.tm_hour << ":"
        << setw(2) << ltm.tm_min << endl;
    outFile << "===================================================================================" << endl;
    for (int i = 0; i < boxSizes.size(); i++) {
        outFile << i + 1 << ") \t" << "Ваза " << " размером " << boxSizes[i] << ":" << endl << "\t";
        for (int j = 0; j < boxes[i].size(); j++) {
            outFile << boxes[i][j] << " ";
        }
        outFile << endl;
    }
    outFile << "===================================================================================" << endl;
    // Вывод информации о не влезших элементах
    if (!notFitElements.empty()) {
        outFile << "Не все цветы можно разместить в вазы: " << endl;
        for (auto it : notFitElements) {
            outFile << "\t" << "Вид цветка: " << it.first << ". Количество: " << it.second << " шт." << endl;
        }
        outFile << "===================================================================================" << endl;
    }
    outFile.close();
}

void Start();// Основное
void About();// Описание

int main()
{
    RenderWindow win(VideoMode(1280, 720), "ARM");// Создание окна
    win.setFramerateLimit(60);//Количество кадров в секунду

    // Загрузка иконки
    Image icon;
    icon.loadFromFile("Image/icon1.png");
    win.setIcon(1200, 1084, icon.getPixelsPtr());

    win.setMouseCursorVisible(false);// Отключение видимости курсора

    Music music;// Вщзпроизведение музыкальной дороожки
    music.openFromFile("music.ogg");
    music.play();
    music.setLoop(true);
    music.setVolume(2.f);

    // Создание рамки
    Image image;
    image.loadFromFile("Image/panel.jpg");
    image.createMaskFromColor(image.getPixel(0, 0));
    Texture texture;
    texture.loadFromImage(image);
    Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(Vector2f(330, 50));

    // Создание движущегося фона
    Texture textureFon;
    textureFon.loadFromFile("Image/fon.jpg");
    RectangleShape fon(Vector2f(1280, 720));
    fon.setTexture(&textureFon);

    RectangleShape fon2(Vector2f(1280, 720));
    fon2.setTexture(&textureFon);
    fon2.setPosition(Vector2f(1280, 0));

    // Загрузка шрифта
    Font font;
    if (!font.loadFromFile("font/myFont.otf")) return 5;

    // Создание заголовка
    Text Titul;
    Titul.setFont(font);
    TextFormat Ftext;
    Ftext.size_font = 45;
    Ftext.menu_text_color = Color{ 20, 20, 20};
    Ftext.bord = 0;
    InitText(Titul, 450, 250, L"АРМ ФЛОРИСТА", Ftext);

    //Основное меню
    vector<String> name_menu{ L"НАЧАТЬ", L"ОПИСАНИЕ", L"ВЫЙТИ" };// Название пунктов меню
    game::MyMenu mymenu(win, 625 , 320, 35, 35, name_menu);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu.setColorTextMenu(Color(200,200,200), Color(15,15,15), Color::Black);// Установка цвета отображения меню
    mymenu.AlignMenu(2); //Выравнивание пунктов меню

    Vector2f pos;// Координаты объектов
    Clock clock;    // Таймер
    float time;

    // Основной цикл 
    while (win.isOpen())
    {
        // Обработка событий
        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) //Для закрытия через крестик
                 win.close();
            if (event.type == Event::KeyReleased)
            {
                // События выбра пунктов меню
                if (event.key.code == Keyboard::Up) { mymenu.MoveUp(); }        // вверх
                if (event.key.code == Keyboard::Down) { mymenu.MoveDown(); }    // вниз
                if (event.key.code == Keyboard::Return)                         // ввод
                {
                    // Переходим на выбранный пункт меню
                    switch (mymenu.getSelectedMenuNumber())
                    {
                    case 0:Start();  break;
                    case 1:About();  break;
                    case 2:win.close(); break;
                    default:break;
                    }
                }
            }
        }
        // Обновление таймера
        time = clock.getElapsedTime().asMicroseconds();
        time = time / 15000;
        clock.restart();

        // Движение фона
        fon.move(-0.2 * time, 0);
        pos = fon.getPosition();
        if (pos.x < -1280) fon.setPosition(1280, pos.y);

        fon2.move(-0.2 * time, 0);
        pos = fon2.getPosition();
        if (pos.x < -1280) fon2.setPosition(1280, pos.y);

        win.clear();// Очистка экрана
        win.draw(fon);// Отрисовка элементов игры
        win.draw(fon2);
        win.draw(sprite);
        mymenu.draw();
        win.draw(Titul);
        win.display();// Обновление экрана
    }
    return 0;
}

// Основное
void Start()
{
    RenderWindow win(VideoMode(1280, 720), "ARM"); // Создание окна
    win.setFramerateLimit(60);
    // Загрузка иконки
    Image icon;
    icon.loadFromFile("Image/icon1.png");
    win.setIcon(1200, 1084, icon.getPixelsPtr());
    // Загрузка шрифта
    Font font;
    font.loadFromFile("font/myFont2.ttf");

    // Цветы сбоку
    Image image;
    image.loadFromFile("Image/panel2.jpg");
    image.createMaskFromColor(image.getPixel(30, 30));
    Texture texture;
    texture.loadFromImage(image);
    Sprite panel;
    panel.setTexture(texture);
    panel.setPosition(Vector2f(595, 0));

    // Создание рамки вокруг текста
    Image image2;
    image2.loadFromFile("Image/panel3.jpg");
    image2.createMaskFromColor(image2.getPixel(0, 0));
    Texture texture2;
    texture2.loadFromImage(image2);
    Sprite panel2;
    panel2.setTexture(texture2);
    panel2.setPosition(Vector2f(60, 60));
    panel2.setScale(550 / panel2.getLocalBounds().width, 350 / panel2.getLocalBounds().height);

    // Создание движущегося фона
    Texture textureFon;
    textureFon.loadFromFile("Image/fon.jpg");
    RectangleShape fon(Vector2f(1280, 720));
    fon.setTexture(&textureFon);
    RectangleShape fon2(Vector2f(1280, 720));
    fon2.setTexture(&textureFon);
    fon2.setPosition(Vector2f(1280, 0));
    
    // Создание заголовка
    Text Titul;
    Titul.setFont(font);
    TextFormat Ftext;
    Ftext.size_font = 35;
    Ftext.menu_text_color = Color{ 20, 20, 20 };
    Ftext.bord = 0;
    InitText(Titul, 123, 100, L"ВЫБЕРИТЕ НУЖНЫЙ ПУНКТ", Ftext);

    //Основное меню
    vector<String> name_menu{ L"Управление цветами", L"Управление вазами", L"Получить результат", L"Назад"}; // Название пунктов меню
    game::MyMenu mymenu(win, 330, 170, 35, 45, name_menu);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu.setColorTextMenu(Color(200, 200, 200), Color(15, 15, 15), Color::Black);// Установка цвета отображения меню
    mymenu.AlignMenu(2);//Выпавнивание пунктов меню
    
    //Фон основного мею
    RectangleShape button(Vector2f(470, 290));
    button.setFillColor(Color::White);
    button.setPosition(100, 90);
    button.setOutlineThickness(1);
    button.setOutlineColor(Color(100, 100, 100));

//ДЛЯ УПРАВЛЕНИЯ ЦВЕТАМИ
    bool menuExpanded = false;//Окошко после нажатия на управление цветами
    int x = 640;
    int y = 90;
//ПЕРВОЕ ОКОШКО С ЦВЕТКОМ
    RectangleShape for_flovers1(Vector2f(330, 50));//Для конкретного цветка окошко
    for_flovers1.setFillColor(Color::White);
    for_flovers1.setOutlineThickness(1);
    for_flovers1.setOutlineColor(Color(100, 100, 100));
    for_flovers1.setPosition(x, y );

    vector<String> name_menu1{ L"+", L"-" }; // Название пунктов меню
    game::MyMenu mymenu1(win, x+305, y, 30, 15, name_menu1);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu1.setColorTextMenu(Color::Black, Color::Black, Color::Black);// Установка цвета отображения меню
    mymenu1.AlignMenu(2);//Выпавнивание пунктов меню

    String f1 = L"1. Гвоздика садовая";// Название конкретного цветка
    Text name_flovers1;
    name_flovers1.setFont(font);
    name_flovers1.setFillColor(Color(15,15,15));
    name_flovers1.setString(f1);
    name_flovers1.setCharacterSize(30);
    name_flovers1.setPosition(for_flovers1.getPosition().x + 5, for_flovers1.getPosition().y+ 5);

    RectangleShape for_zifri1(Vector2f(30, 30));//Колво цветов окошко
    for_zifri1.setFillColor(Color::White);
    for_zifri1.setPosition(for_flovers1.getPosition().x + 260, for_flovers1.getPosition().y + 10);
    for_zifri1.setOutlineThickness(1);
    for_zifri1.setOutlineColor(Color(100, 100, 100));

    int z1 = 0;// Кол во цветов цифры
    Text zifri1;
    zifri1.setFont(font);
    zifri1.setFillColor(Color::Black);
    zifri1.setString(to_string(z1));
    zifri1.setCharacterSize(25);
    zifri1.setOutlineThickness(0.1);
    zifri1.setOutlineColor(Color::Black);
    zifri1.setPosition(for_zifri1.getPosition().x + 2, for_zifri1.getPosition().y);

//ВТОРОЕ ОКОШКО С ЦВЕТКОМ
    RectangleShape for_flovers2(Vector2f(330, 50));//Для конкретного цветка окошко
    for_flovers2.setFillColor(Color::White);
    for_flovers2.setOutlineThickness(1);
    for_flovers2.setOutlineColor(Color(100, 100, 100));
    for_flovers2.setPosition(x , y = y + 50);

    vector<String> name_menu2{ L"+", L"-" }; // Название пунктов меню
    game::MyMenu mymenu2(win, x + 305, y, 30, 15, name_menu2);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu2.setColorTextMenu(Color::Black, Color::Black, Color::Black);// Установка цвета отображения меню
    mymenu2.AlignMenu(2);//Выпавнивание пунктов меню

    String f2 = L"2. Ландыши майские";// Название конкретного цветка
    Text name_flovers2;
    name_flovers2.setFont(font);
    name_flovers2.setFillColor(Color(15, 15, 15));
    name_flovers2.setString(f2);
    name_flovers2.setCharacterSize(30);
    name_flovers2.setPosition(for_flovers2.getPosition().x + 5, for_flovers2.getPosition().y + 5);

    RectangleShape for_zifri2(Vector2f(30, 30));//Колво цветов окошко
    for_zifri2.setFillColor(Color::White);
    for_zifri2.setPosition(for_flovers2.getPosition().x + 260, for_flovers2.getPosition().y + 10);
    for_zifri2.setOutlineThickness(1);
    for_zifri2.setOutlineColor(Color(100, 100, 100));

    int z2 = 0;// Кол во цветов цифры
    Text zifri2;
    zifri2.setFont(font);
    zifri2.setFillColor(Color::Black);
    zifri2.setString(to_string(z2));
    zifri2.setCharacterSize(25);
    zifri2.setOutlineThickness(0.1);
    zifri2.setOutlineColor(Color::Black);
    zifri2.setPosition(for_zifri2.getPosition().x + 2, for_zifri2.getPosition().y);


 //3 ОКОШКО С ЦВЕТКОМ
    RectangleShape for_flovers3(Vector2f(330, 50));//Для конкретного цветка окошко
    for_flovers3.setFillColor(Color::White);
    for_flovers3.setOutlineThickness(1);
    for_flovers3.setOutlineColor(Color(100, 100, 100));
    for_flovers3.setPosition(x, y = y + 50);

    vector<String> name_menu3{ L"+", L"-" }; // Название пунктов меню
    game::MyMenu mymenu3(win, x + 305, y, 30, 15, name_menu3);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu3.setColorTextMenu(Color::Black, Color::Black, Color::Black);// Установка цвета отображения меню
    mymenu3.AlignMenu(2);//Выпавнивание пунктов меню

    String f3 = L"3. Лилия азиатская";// Название конкретного цветка
    Text name_flovers3;
    name_flovers3.setFont(font);
    name_flovers3.setFillColor(Color(15, 15, 15));
    name_flovers3.setString(f3);
    name_flovers3.setCharacterSize(30);
    name_flovers3.setPosition(for_flovers3.getPosition().x + 5, for_flovers3.getPosition().y + 5);

    RectangleShape for_zifri3(Vector2f(30, 30));//Колво цветов окошко
    for_zifri3.setFillColor(Color::White);
    for_zifri3.setPosition(for_flovers3.getPosition().x + 260, for_flovers3.getPosition().y + 10);
    for_zifri3.setOutlineThickness(1);
    for_zifri3.setOutlineColor(Color(100, 100, 100));

    int z3 = 0;// Кол во цветов цифры
    Text zifri3;
    zifri3.setFont(font);
    zifri3.setFillColor(Color::Black);
    zifri3.setString(to_string(z3));
    zifri3.setCharacterSize(25);
    zifri3.setOutlineThickness(0.1);
    zifri3.setOutlineColor(Color::Black);
    zifri3.setPosition(for_zifri3.getPosition().x + 2, for_zifri3.getPosition().y);


//4 ОКОШКО С ЦВЕТКОМ
    RectangleShape for_flovers4(Vector2f(330, 50));//Для конкретного цветка окошко
    for_flovers4.setFillColor(Color::White);
    for_flovers4.setOutlineThickness(1);
    for_flovers4.setOutlineColor(Color(100, 100, 100));
    for_flovers4.setPosition(x, y = y + 50);

    vector<String> name_menu4{ L"+", L"-" }; // Название пунктов меню
    game::MyMenu mymenu4(win, x + 305, y, 30, 15, name_menu4);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu4.setColorTextMenu(Color::Black, Color::Black, Color::Black);// Установка цвета отображения меню
    mymenu4.AlignMenu(2);//Выпавнивание пунктов меню

    String f4 = L"4. Нарциссы белые";// Название конкретного цветка
    Text name_flovers4;
    name_flovers4.setFont(font);
    name_flovers4.setFillColor(Color(15, 15, 15));
    name_flovers4.setString(f4);
    name_flovers4.setCharacterSize(30);
    name_flovers4.setPosition(for_flovers4.getPosition().x + 5, for_flovers4.getPosition().y + 5);

    RectangleShape for_zifri4(Vector2f(30, 30));//Колво цветов окошко
    for_zifri4.setFillColor(Color::White);
    for_zifri4.setPosition(for_flovers4.getPosition().x + 260, for_flovers4.getPosition().y + 10);
    for_zifri4.setOutlineThickness(1);
    for_zifri4.setOutlineColor(Color(100, 100, 100));

    int z4 = 0;// Кол во цветов цифры
    Text zifri4;
    zifri4.setFont(font);
    zifri4.setFillColor(Color::Black);
    zifri4.setString(to_string(z4));
    zifri4.setCharacterSize(25);
    zifri4.setOutlineThickness(0.1);
    zifri4.setOutlineColor(Color::Black);
    zifri4.setPosition(for_zifri4.getPosition().x + 2, for_zifri4.getPosition().y);

//5 ОКОШКО С ЦВЕТКОМ
    RectangleShape for_flovers5(Vector2f(330, 50));//Для конкретного цветка окошко
    for_flovers5.setFillColor(Color::White);
    for_flovers5.setOutlineThickness(1);
    for_flovers5.setOutlineColor(Color(100, 100, 100));
    for_flovers5.setPosition(x, y = y + 50);

    vector<String> name_menu5{ L"+", L"-" }; // Название пунктов меню
    game::MyMenu mymenu5(win, x + 305, y, 30, 15, name_menu5);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu5.setColorTextMenu(Color::Black, Color::Black, Color::Black);// Установка цвета отображения меню
    mymenu5.AlignMenu(2);//Выпавнивание пунктов меню

    String f5 = L"5. Pозы штамбовые";// Название конкретного цветка
    Text name_flovers5;
    name_flovers5.setFont(font);
    name_flovers5.setFillColor(Color(15, 15, 15));
    name_flovers5.setString(f5);
    name_flovers5.setCharacterSize(30);
    name_flovers5.setPosition(for_flovers5.getPosition().x + 5, for_flovers5.getPosition().y + 5);

    RectangleShape for_zifri5(Vector2f(30, 30));//Колво цветов окошко
    for_zifri5.setFillColor(Color::White);
    for_zifri5.setPosition(for_flovers5.getPosition().x + 260, for_flovers5.getPosition().y + 10);
    for_zifri5.setOutlineThickness(1);
    for_zifri5.setOutlineColor(Color(100, 100, 100));

    int z5 = 0;// Кол во цветов цифры
    Text zifri5;
    zifri5.setFont(font);
    zifri5.setFillColor(Color::Black);
    zifri5.setString(to_string(z5));
    zifri5.setCharacterSize(25);
    zifri5.setOutlineThickness(0.1);
    zifri5.setOutlineColor(Color::Black);
    zifri5.setPosition(for_zifri5.getPosition().x + 2, for_zifri5.getPosition().y);


// ДЛЯ УПРАВЛЕНИЯ ВАЗАМИ
    bool menuExpanded2 = false;
    x = 640;
    y = 90;
//ПЕРВОЕ ОКОШКО С ВАЗАМИ
    RectangleShape for_vazi1(Vector2f(330, 50));//Для конкретного цветка окошко
    for_vazi1.setFillColor(Color::White);
    for_vazi1.setOutlineThickness(1);
    for_vazi1.setOutlineColor(Color(100, 100, 100));
    for_vazi1.setPosition(x, y);

    vector<String> name_menu_vasi1{ L"+", L"-" }; // Название пунктов меню
    game::MyMenu mymenu_vasi1(win, x + 305, y, 30, 15, name_menu_vasi1);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu_vasi1.setColorTextMenu(Color::Black, Color::Black, Color::Black);// Установка цвета отображения меню
    mymenu_vasi1.AlignMenu(2);//Выпавнивание пунктов меню

    String v1 = L"Малая ваза (3 цв.)";// Название конкретного цветка
    Text name_vasi1;
    name_vasi1.setFont(font);
    name_vasi1.setFillColor(Color(15, 15, 15));
    name_vasi1.setString(v1);
    name_vasi1.setCharacterSize(30);
    name_vasi1.setPosition(for_vazi1.getPosition().x + 5, for_vazi1.getPosition().y + 5);

    RectangleShape for_vasi1(Vector2f(30, 30));//Колво цветов окошко
    for_vasi1.setFillColor(Color::White);
    for_vasi1.setPosition(for_vazi1.getPosition().x + 260, for_vazi1.getPosition().y + 10);
    for_vasi1.setOutlineThickness(1);
    for_vasi1.setOutlineColor(Color(100, 100, 100));

    int zv1 = 0;// Кол во цветов цифры
    Text zifri_vazi1;
    zifri_vazi1.setFont(font);
    zifri_vazi1.setFillColor(Color::Black);
    zifri_vazi1.setString(to_string(zv1));
    zifri_vazi1.setCharacterSize(25);
    zifri_vazi1.setOutlineThickness(0.1);
    zifri_vazi1.setOutlineColor(Color::Black);
    zifri_vazi1.setPosition(for_vasi1.getPosition().x + 2, for_vasi1.getPosition().y);

 //2 ОКОШКО С ВАЗАМИ
    RectangleShape for_vazi2(Vector2f(330, 50));//Для конкретного цветка окошко
    for_vazi2.setFillColor(Color::White);
    for_vazi2.setOutlineThickness(1);
    for_vazi2.setOutlineColor(Color(100, 100, 100));
    for_vazi2.setPosition(x, y = y+50);

    vector<String> name_menu_vasi2{ L"+", L"-" }; // Название пунктов меню
    game::MyMenu mymenu_vasi2(win, x + 305, y, 30, 15, name_menu_vasi2);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu_vasi2.setColorTextMenu(Color::Black, Color::Black, Color::Black);// Установка цвета отображения меню
    mymenu_vasi2.AlignMenu(2);//Выпавнивание пунктов меню

    String v2 = L"Средняя ваза (5 цв.)";// Название конкретного цветка
    Text name_vasi2;
    name_vasi2.setFont(font);
    name_vasi2.setFillColor(Color(15, 15, 15));
    name_vasi2.setString(v2);
    name_vasi2.setCharacterSize(30);
    name_vasi2.setPosition(for_vazi2.getPosition().x + 5, for_vazi2.getPosition().y + 5);

    RectangleShape for_vasi2(Vector2f(30, 30));//Колво цветов окошко
    for_vasi2.setFillColor(Color::White);
    for_vasi2.setPosition(for_vazi2.getPosition().x + 260, for_vazi2.getPosition().y + 10);
    for_vasi2.setOutlineThickness(1);
    for_vasi2.setOutlineColor(Color(100, 100, 100));

    int zv2 = 0;// Кол во цветов цифры
    Text zifri_vazi2;
    zifri_vazi2.setFont(font);
    zifri_vazi2.setFillColor(Color::Black);
    zifri_vazi2.setString(to_string(zv2));
    zifri_vazi2.setCharacterSize(25);
    zifri_vazi2.setOutlineThickness(0.1);
    zifri_vazi2.setOutlineColor(Color::Black);
    zifri_vazi2.setPosition(for_vasi2.getPosition().x + 2, for_vasi2.getPosition().y);

 //3 ОКОШКО С ВАЗАМИ
    RectangleShape for_vazi3(Vector2f(330, 50));//Для конкретного цветка окошко
    for_vazi3.setFillColor(Color::White);
    for_vazi3.setOutlineThickness(1);
    for_vazi3.setOutlineColor(Color(100, 100, 100));
    for_vazi3.setPosition(x, y= y+50);

    vector<String> name_menu_vasi3{ L"+", L"-" }; // Название пунктов меню
    game::MyMenu mymenu_vasi3(win, x + 305, y, 30, 15, name_menu_vasi3);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu_vasi3.setColorTextMenu(Color::Black, Color::Black, Color::Black);// Установка цвета отображения меню
    mymenu_vasi3.AlignMenu(2);//Выпавнивание пунктов меню

    String v3 = L"Большая ваза (10 цв.)";// Название конкретного цветка
    Text name_vasi3;
    name_vasi3.setFont(font);
    name_vasi3.setFillColor(Color(15, 15, 15));
    name_vasi3.setString(v3);
    name_vasi3.setCharacterSize(30);
    name_vasi3.setPosition(for_vazi3.getPosition().x + 5, for_vazi3.getPosition().y + 5);

    RectangleShape for_vasi3(Vector2f(30, 30));//Колво цветов окошко
    for_vasi3.setFillColor(Color::White);
    for_vasi3.setPosition(for_vazi3.getPosition().x + 260, for_vazi3.getPosition().y + 10);
    for_vasi3.setOutlineThickness(1);
    for_vasi3.setOutlineColor(Color(100, 100, 100));

    int zv3 = 0;// Кол во цветов цифры
    Text zifri_vazi3;
    zifri_vazi3.setFont(font);
    zifri_vazi3.setFillColor(Color::Black);
    zifri_vazi3.setString(to_string(zv3));
    zifri_vazi3.setCharacterSize(25);
    zifri_vazi3.setOutlineThickness(0.1);
    zifri_vazi3.setOutlineColor(Color::Black);
    zifri_vazi3.setPosition(for_vasi3.getPosition().x + 2, for_vasi3.getPosition().y);

//ОКОШКО С РЕЗУЛЬТАТАМИ
    bool menuExpanded3 = false;
    x = 640;
    y = 90;
    RectangleShape for_rez1(Vector2f(470, 125));//Для конкретного цветка окошко
    for_rez1.setFillColor(Color::White);
    for_rez1.setOutlineThickness(1);
    for_rez1.setOutlineColor(Color(100, 100, 100));
    for_rez1.setPosition(x, y);

    String r1 = L"Рекомендации по расположению цветов\nзаписаны на ваш компьютер в файл:\nresult.txt";// Название конкретного цветка
    Text rez1;
    rez1.setFont(font);
    rez1.setFillColor(Color(15, 15, 15));
    rez1.setString(r1);
    rez1.setCharacterSize(30);
    rez1.setPosition(for_rez1.getPosition().x + 5, for_rez1.getPosition().y + 5);

    Vector2f pos;// Координаты объектов
    Clock clock;// Таймер
    float time;

    // Основной цикл
    while (win.isOpen())
    {
        // Обработка событий
        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == Event::Closed) //Для закрытия через крестик
                win.close();

            if (event.type == Event::KeyReleased)
            {
                // Обработчик событий для основного меню окна (управление цветами, управление вазами, назад)
                if (event.key.code == Keyboard::Up) { menuExpanded = 0; menuExpanded2 = 0; menuExpanded3 = 0;  mymenu.MoveUp(); }        // вверх
                if (event.key.code == Keyboard::Down) { menuExpanded = 0; menuExpanded2 = 0; menuExpanded3 = 0; mymenu.MoveDown(); }    // вниз
                if (event.key.code == Keyboard::Return)
                {
                    // Переходим на выбранный пункт меню
                    switch (mymenu.getSelectedMenuNumber())
                    {
                    case 0:
                        menuExpanded = true;
                        break;
                    case 1:
                        menuExpanded2 = true;
                        break;
                    case 2: 
                        menuExpanded3 = true;
                        
                        // Вызываем функцию расфасовки элементов
                        packElements(z1, z2, z3, z4, z5, zv1, zv2, zv3);

                        break;
                    case 3:
                        win.close();
                        break;
                    default:
                        break;
                    }
                }
            }
            // Обработчик событий для меню Управления цветами

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.key.code == Mouse::Left)
                {
                    if (menuExpanded)
                    {
                        // Получаем позицию мыши
                        Vector2f mousePos = win.mapPixelToCoords(Mouse::getPosition(win));
                        int w = 50;
                        //ПЕРВОЕ ОКОШКО С ЦВЕТКОМ
                        if (for_flovers1.getGlobalBounds().contains(mousePos))// Проверяем, был ли клик внутри области меню
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 && mousePos.y <= 115)// Проверяем, был ли клик на кнопке "+"
                            {
                                z1 = z1 + 1;// Выполняем действия для кнопки "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 && mousePos.y <= 130)// Проверяем, был ли клик на кнопке "-"
                            {
                                if (z1 > 0) {// Выполняем действия для кнопки "-"
                                    z1 = z1 - 1;
                                }
                            }
                            zifri1.setString(to_string(z1));// Обновляем текстовое поле с количеством
                        }
                        //ВТОРОЕ ОКОШКО С ЦВЕТКОМ
                        if (for_flovers2.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w && mousePos.y <= 115 + w)// Проверяем, был ли клик на кнопке "+"
                            {
                                z2 = z2 + 1;// Выполняем действия для кнопки "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w && mousePos.y <= 130 + w)// Проверяем, был ли клик на кнопке "-"
                            {
                                if (z2 > 0) { // Выполняем действия для кнопки "-"
                                    z2 = z2 - 1;
                                }
                            }
                            zifri2.setString(to_string(z2));// Обновляем текстовое поле с количеством
                        }
                        //3 ОКОШКО С ЦВЕТКОМ
                        if (for_flovers3.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w * 2 && mousePos.y <= 115 + w * 2)// Проверяем, был ли клик на кнопке "+"
                            {
                                z3 = z3 + 1;// Выполняем действия для кнопки "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w * 2 && mousePos.y <= 130 + w * 2)// Проверяем, был ли клик на кнопке "-"
                            {
                                if (z3 > 0) { // Выполняем действия для кнопки "-"
                                    z3 = z3 - 1;
                                }
                            }
                            zifri3.setString(to_string(z3));// Обновляем текстовое поле с количеством
                        }
                        //4 ОКОШКО С ЦВЕТКОМ
                        if (for_flovers4.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w * 3 && mousePos.y <= 115 + w * 3)// Проверяем, был ли клик на кнопке "+"
                            {
                                z4 = z4 + 1;// Выполняем действия для кнопки "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w * 3 && mousePos.y <= 130 + w * 3)// Проверяем, был ли клик на кнопке "-"
                            {
                                if (z4 > 0) { // Выполняем действия для кнопки "-"
                                    z4 = z4 - 1;
                                }
                            }
                            zifri4.setString(to_string(z4));// Обновляем текстовое поле с количеством
                        }
                        //5 ОКОШКО С ЦВЕТКОМ
                        if (for_flovers5.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w * 4 && mousePos.y <= 115 + w * 4)// Проверяем, был ли клик на кнопке "+"
                            {
                                z5 = z5 + 1;// Выполняем действия для кнопки "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w * 4 && mousePos.y <= 130 + w * 4)// Проверяем, был ли клик на кнопке "-"
                            {
                                if (z5 > 0) { // Выполняем действия для кнопки "-"
                                    z5 = z5 - 1;
                                }
                            }
                            zifri5.setString(to_string(z5));// Обновляем текстовое поле с количеством
                        }
                    }

                    //Обработчик событий для меню Управления вазами
                    if (menuExpanded2)
                    {
                        // Получаем позицию мыши
                        Vector2f mousePos = win.mapPixelToCoords(Mouse::getPosition(win));
                        int w = 50;
                        //ПЕРВОЕ ОКОШКО С ВАЗОЙ
                        if (for_vazi1.getGlobalBounds().contains(mousePos))// Проверяем, был ли клик внутри области меню
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 && mousePos.y <= 115)// Проверяем, был ли клик на кнопке "+"
                            {
                                zv1 = zv1 + 1;// Выполняем действия для кнопки "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 && mousePos.y <= 130)// Проверяем, был ли клик на кнопке "-"
                            {
                                if (zv1 > 0) {// Выполняем действия для кнопки "-"
                                    zv1 = zv1 - 1;
                                }
                            }
                            zifri_vazi1.setString(to_string(zv1));// Обновляем текстовое поле с количеством
                        }
                        //ВТОРОЕ ОКОШКО С ВАЗОЙ
                        if (for_vazi2.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w && mousePos.y <= 115 + w)// Проверяем, был ли клик на кнопке "+"
                            {
                                zv2 = zv2 + 1;// Выполняем действия для кнопки "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w && mousePos.y <= 130 + w)// Проверяем, был ли клик на кнопке "-"
                            {
                                if (zv2 > 0) { // Выполняем действия для кнопки "-"
                                    zv2 = zv2 - 1;
                                }
                            }
                            zifri_vazi2.setString(to_string(zv2));// Обновляем текстовое поле с количеством
                        }
                        //3 ОКОШКО С ВАЗОЙ
                        if (for_vazi3.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w * 2 && mousePos.y <= 115 + w * 2)// Проверяем, был ли клик на кнопке "+"
                            {
                                zv3 = zv3 + 1;// Выполняем действия для кнопки "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w * 2 && mousePos.y <= 130 + w * 2)// Проверяем, был ли клик на кнопке "-"
                            {
                                if (zv3 > 0) { // Выполняем действия для кнопки "-"
                                    zv3 = zv3 - 1;
                                }
                            }
                            zifri_vazi3.setString(to_string(zv3));// Обновляем текстовое поле с количеством
                        }


                    }
                }
            }
        }
        // Обновление таймера
        time = clock.getElapsedTime().asMicroseconds();
        time = time / 15000;
        clock.restart();

        // Движение фона
        fon.move(-0.2 * time, 0);
        pos = fon.getPosition();
        if (pos.x < -1280) fon.setPosition(1280, pos.y);
        fon2.move(-0.2 * time, 0);
        pos = fon2.getPosition();
        if (pos.x < -1280) fon2.setPosition(1280, pos.y);

        win.clear();// Очистка экрана
        win.draw(fon); // Отрисовка элементов
        win.draw(fon2);
        win.draw(panel);
        win.draw(panel2);
        win.draw(button);
        mymenu.draw();
        win.draw(Titul);
        if (menuExpanded) {
            win.draw(for_flovers1); win.draw(name_flovers1); mymenu1.draw();  win.draw(for_zifri1); win.draw(zifri1);  
            win.draw(for_flovers2); win.draw(name_flovers2); mymenu2.draw();  win.draw(for_zifri2); win.draw(zifri2);
            win.draw(for_flovers3); win.draw(name_flovers3); mymenu3.draw();  win.draw(for_zifri3); win.draw(zifri3);
            win.draw(for_flovers4); win.draw(name_flovers4); mymenu4.draw();  win.draw(for_zifri4); win.draw(zifri4);
            win.draw(for_flovers5); win.draw(name_flovers5); mymenu5.draw();  win.draw(for_zifri5); win.draw(zifri5);
        }
        if (menuExpanded2) {
            win.draw(for_vazi1); win.draw(name_vasi1); mymenu_vasi1.draw();  win.draw(for_vasi1); win.draw(zifri_vazi1);
            win.draw(for_vazi2); win.draw(name_vasi2); mymenu_vasi2.draw();  win.draw(for_vasi2); win.draw(zifri_vazi2);
            win.draw(for_vazi3); win.draw(name_vasi3); mymenu_vasi3.draw();  win.draw(for_vasi3); win.draw(zifri_vazi3);
        }
        if (menuExpanded3) {
            win.draw(for_rez1); win.draw(rez1);
        }
        win.display();// Обновление экрана
    }
}

// Описание
void About() {
    
    RenderWindow win(VideoMode(1280, 720), "ARM");// Создание окна
    win.setFramerateLimit(60);
    // Загрузка иконки
    Image icon;
    icon.loadFromFile("Image/icon1.png");
    win.setIcon(1200, 1084, icon.getPixelsPtr());
    
    win.setMouseCursorVisible(false);// Отключение видимости курсора

    // Серый фон для текста
    Image image;
    image.loadFromFile("Image/panel4.jpg");
    image.createMaskFromColor(image.getPixel(0, 0));
    Texture texture;
    texture.loadFromImage(image);
    Sprite panel;
    panel.setTexture(texture);
    panel.setPosition(Vector2f(0, 0));
    panel.setScale(1280 / panel.getLocalBounds().width, 720 / panel.getLocalBounds().height);

    // Создание движущегося фона
    Texture textureFon;
    textureFon.loadFromFile("Image/fon2.jpg");
    RectangleShape fon(Vector2f(1280, 720));
    fon.setTexture(&textureFon);

    RectangleShape fon2(Vector2f(1280, 720));
    fon2.setTexture(&textureFon);
    fon2.setPosition(Vector2f(1280, 0));

    // Загрузка шрифта
    Font font;
    font.loadFromFile("font/myFont2.ttf");

    // Создание текста с названием экрана
    Text Titul;
    Titul.setFont(font);
    TextFormat Ftext;
    Ftext.size_font = 35;
    Ftext.menu_text_color = Color{ 10, 10, 10 };
    Ftext.bord = 0;
    InitText(Titul, 400, 160, L"ДОПОЛНИТЕЛЬНЫЕ СВЕДЕНЬЯ:", Ftext);

    // Загрузка шрифта
    Font font2;
    font2.loadFromFile("font/myFont2.ttf");

    // Создание текста с названием экрана
    Text Titul2;
    Titul2.setFont(font2);
    TextFormat Ftext2;
    Ftext2.size_font = 4;
    Ftext2.menu_text_color = Color( 20, 20, 20 );
    Ftext2.bord = 0;
    InitText(Titul2, 200, 220, L"\tДанное приложене представляет из себя автоматизированное \nрабочее место флориста.\n\n\tОно способно упостить и ускорить работу флориста во время \nпоступления крупных партий.\n\n\tС его помощью можно отслеживать дату поставки товара и \nсоответственно контролировать качество продукции.", Ftext);

    //Переход назад (реализовано через меню)
    vector<String> name_menu2{ L"Нажмите Enter" };// Название пунктов меню
    game::MyMenu mymenu(win, 930, 560, 20, 0, name_menu2);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu.setColorTextMenu(Color(20, 20, 20), Color(20, 20, 20), Color(20,20,20)); // Установка цвета отображения меню
    mymenu.AlignMenu(3);

    Vector2f pos;// Координаты объектов
    Clock clock;// Таймер
    float time;

    // Основной цикл
    while (win.isOpen())
    {
        // Обработка событий
        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) //Закрытие через крестик
                win.close();

            if (event.type == Event::KeyReleased)
            {
                // События выбра пунктов меню
                if (event.key.code == Keyboard::Up) { mymenu.MoveUp(); }        // вверх
                if (event.key.code == Keyboard::Down) { mymenu.MoveDown(); }    // вниз
                if (event.key.code == Keyboard::Return)                         // ввод
                {
                    // Переходим на выбранный пункт меню
                    switch (mymenu.getSelectedMenuNumber())
                    {
                    case 0:win.close(); break;
                    default:break;
                    }
                }
            }
        }
        // Обновление таймера 
        time = clock.getElapsedTime().asMicroseconds();
        time = time / 8000;
        clock.restart();

        // Движение фона
        fon.move(-0.2 * time, 0);
        pos = fon.getPosition();
        if (pos.x < -1280) fon.setPosition(1280, pos.y);

        fon2.move(-0.2 * time, 0);
        pos = fon2.getPosition();
        if (pos.x < -1280) fon2.setPosition(1280, pos.y);

        win.clear();// Очистка экрана
        win.draw(fon);// Отрисовка элементов
        win.draw(fon2);
        win.draw(panel);
        mymenu.draw();
        win.draw(Titul);
        win.draw(Titul2);
        win.display();// Обновление экрана
    }
}