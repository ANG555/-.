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
    int size_font = 60; // ������ ������
    Color menu_text_color = Color::White; // ���� ������
    float bord = 0.0f; // ������� �������
    Color border_color = Color::Black; // ���� �������
};// ��������� ��� �������� ���������� �������������� ������

// ������� ��������� ������
void InitText(Text& mtext, float xpos, float ypos, const String& str, TextFormat Ftext)
{
    // ��������� ���������� ������
    mtext.setCharacterSize(Ftext.size_font);
    mtext.setPosition(xpos, ypos);
    mtext.setString(str);
    mtext.setFillColor(Ftext.menu_text_color);
    mtext.setOutlineThickness(Ftext.bord);
    mtext.setOutlineColor(Ftext.border_color);
}

// ��������� ��� ������������� ��������
struct Element {
    int type; // ��� ��������
    int count; // ���������� ���������
    int not_type; // ��� ��������, ������� �� ������ ���� � ����� ������� � ������
};

// ������� ��� ��������, ����� �� �������� ������� � �������
bool canAddToBox(vector<int>& box, int type, int not_type) {
    for (int t : box) {
        if (t == not_type) {
            return false; // ������ �������, ������� �� ������ ���� ������
        }
    }
    return true;
}

// ������� ��� ���������� ��������� �� ��������
void packElements(int z1, int z2, int z3, int z4, int z5, int zv1, int zv2, int zv3) {


    int boxCount10 = zv3, boxCount5 = zv2, boxCount3 = zv1;

    // ������� ������ boxSizes
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
                {1, z1, 5},  // ��� 1, 4 ��������, �� ������ ���� � ����� 2
                {2, z2, 0},  // ��� 2, 3 ��������, �� ������ ���� � ����� 1
                {3, z3, 4},  // ��� 3, 2 ��������, �� ������ ���� � ����� 3
                {4, z4, 3},  // ��� 4, 5 ���������, �� ������ ���� � ����� 2
                {5, z5, 1},  // ��� 5, 1 �������, �� ������ ���� � ����� 3
    };

    // ������� �������
    vector<vector<int>> boxes(boxSizes.size());

    // ���������� map ��� �������� �� ������� ���������
    map<int, int> notFitElements;

    // ���������� ��� ��������
    for (auto& elem : elements) {
        int type = elem.type;    // ��� �������� ��������
        int count = elem.count;  // ���������� �������� ��������
        int not_type = elem.not_type; // ���, ������� �� ������ ���� � ����� �������

        // ������������� ������� �� ��������
        for (int i = 0; i < count; i++) {
            bool placed = false;
            for (int j = 0; j < boxSizes.size(); j++) {
                if (boxes[j].size() < boxSizes[j] && canAddToBox(boxes[j], type, not_type)) {
                    boxes[j].push_back(type);
                    placed = true;
                    break;
                }
            }
            // ���� ��� ����� �� � ����� ������� ��� ���������� �������, ����������� ������� ��� ������� ����
            if (!placed) {
                notFitElements[type]++;
            }
        }
    }

    // ���������� ���������� ������� � ����
    ofstream outFile("result.txt");

    // �������� ������� ����� � ����
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    // ����������� ���� � �����
    outFile << "===================================================================================" << endl;
    outFile << "���� � ����� ��������: "
        << ltm.tm_mday << "/"
        << ltm.tm_mon + 1 << "/"
        << ltm.tm_year + 1900 << " "
        << setfill('0') << setw(2) << ltm.tm_hour << ":"
        << setw(2) << ltm.tm_min << endl;
    outFile << "===================================================================================" << endl;
    for (int i = 0; i < boxSizes.size(); i++) {
        outFile << i + 1 << ") \t" << "���� " << " �������� " << boxSizes[i] << ":" << endl << "\t";
        for (int j = 0; j < boxes[i].size(); j++) {
            outFile << boxes[i][j] << " ";
        }
        outFile << endl;
    }
    outFile << "===================================================================================" << endl;
    // ����� ���������� � �� ������� ���������
    if (!notFitElements.empty()) {
        outFile << "�� ��� ����� ����� ���������� � ����: " << endl;
        for (auto it : notFitElements) {
            outFile << "\t" << "��� ������: " << it.first << ". ����������: " << it.second << " ��." << endl;
        }
        outFile << "===================================================================================" << endl;
    }
    outFile.close();
}

void Start();// ��������
void About();// ��������

int main()
{
    RenderWindow win(VideoMode(1280, 720), "ARM");// �������� ����
    win.setFramerateLimit(60);//���������� ������ � �������

    // �������� ������
    Image icon;
    icon.loadFromFile("Image/icon1.png");
    win.setIcon(1200, 1084, icon.getPixelsPtr());

    win.setMouseCursorVisible(false);// ���������� ��������� �������

    Music music;// ��������������� ����������� ��������
    music.openFromFile("music.ogg");
    music.play();
    music.setLoop(true);
    music.setVolume(2.f);

    // �������� �����
    Image image;
    image.loadFromFile("Image/panel.jpg");
    image.createMaskFromColor(image.getPixel(0, 0));
    Texture texture;
    texture.loadFromImage(image);
    Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(Vector2f(330, 50));

    // �������� ����������� ����
    Texture textureFon;
    textureFon.loadFromFile("Image/fon.jpg");
    RectangleShape fon(Vector2f(1280, 720));
    fon.setTexture(&textureFon);

    RectangleShape fon2(Vector2f(1280, 720));
    fon2.setTexture(&textureFon);
    fon2.setPosition(Vector2f(1280, 0));

    // �������� ������
    Font font;
    if (!font.loadFromFile("font/myFont.otf")) return 5;

    // �������� ���������
    Text Titul;
    Titul.setFont(font);
    TextFormat Ftext;
    Ftext.size_font = 45;
    Ftext.menu_text_color = Color{ 20, 20, 20};
    Ftext.bord = 0;
    InitText(Titul, 450, 250, L"��� ��������", Ftext);

    //�������� ����
    vector<String> name_menu{ L"������", L"��������", L"�����" };// �������� ������� ����
    game::MyMenu mymenu(win, 625 , 320, 35, 35, name_menu);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu.setColorTextMenu(Color(200,200,200), Color(15,15,15), Color::Black);// ��������� ����� ����������� ����
    mymenu.AlignMenu(2); //������������ ������� ����

    Vector2f pos;// ���������� ��������
    Clock clock;    // ������
    float time;

    // �������� ���� 
    while (win.isOpen())
    {
        // ��������� �������
        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) //��� �������� ����� �������
                 win.close();
            if (event.type == Event::KeyReleased)
            {
                // ������� ����� ������� ����
                if (event.key.code == Keyboard::Up) { mymenu.MoveUp(); }        // �����
                if (event.key.code == Keyboard::Down) { mymenu.MoveDown(); }    // ����
                if (event.key.code == Keyboard::Return)                         // ����
                {
                    // ��������� �� ��������� ����� ����
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
        // ���������� �������
        time = clock.getElapsedTime().asMicroseconds();
        time = time / 15000;
        clock.restart();

        // �������� ����
        fon.move(-0.2 * time, 0);
        pos = fon.getPosition();
        if (pos.x < -1280) fon.setPosition(1280, pos.y);

        fon2.move(-0.2 * time, 0);
        pos = fon2.getPosition();
        if (pos.x < -1280) fon2.setPosition(1280, pos.y);

        win.clear();// ������� ������
        win.draw(fon);// ��������� ��������� ����
        win.draw(fon2);
        win.draw(sprite);
        mymenu.draw();
        win.draw(Titul);
        win.display();// ���������� ������
    }
    return 0;
}

// ��������
void Start()
{
    RenderWindow win(VideoMode(1280, 720), "ARM"); // �������� ����
    win.setFramerateLimit(60);
    // �������� ������
    Image icon;
    icon.loadFromFile("Image/icon1.png");
    win.setIcon(1200, 1084, icon.getPixelsPtr());
    // �������� ������
    Font font;
    font.loadFromFile("font/myFont2.ttf");

    // ����� �����
    Image image;
    image.loadFromFile("Image/panel2.jpg");
    image.createMaskFromColor(image.getPixel(30, 30));
    Texture texture;
    texture.loadFromImage(image);
    Sprite panel;
    panel.setTexture(texture);
    panel.setPosition(Vector2f(595, 0));

    // �������� ����� ������ ������
    Image image2;
    image2.loadFromFile("Image/panel3.jpg");
    image2.createMaskFromColor(image2.getPixel(0, 0));
    Texture texture2;
    texture2.loadFromImage(image2);
    Sprite panel2;
    panel2.setTexture(texture2);
    panel2.setPosition(Vector2f(60, 60));
    panel2.setScale(550 / panel2.getLocalBounds().width, 350 / panel2.getLocalBounds().height);

    // �������� ����������� ����
    Texture textureFon;
    textureFon.loadFromFile("Image/fon.jpg");
    RectangleShape fon(Vector2f(1280, 720));
    fon.setTexture(&textureFon);
    RectangleShape fon2(Vector2f(1280, 720));
    fon2.setTexture(&textureFon);
    fon2.setPosition(Vector2f(1280, 0));
    
    // �������� ���������
    Text Titul;
    Titul.setFont(font);
    TextFormat Ftext;
    Ftext.size_font = 35;
    Ftext.menu_text_color = Color{ 20, 20, 20 };
    Ftext.bord = 0;
    InitText(Titul, 123, 100, L"�������� ������ �����", Ftext);

    //�������� ����
    vector<String> name_menu{ L"���������� �������", L"���������� ������", L"�������� ���������", L"�����"}; // �������� ������� ����
    game::MyMenu mymenu(win, 330, 170, 35, 45, name_menu);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu.setColorTextMenu(Color(200, 200, 200), Color(15, 15, 15), Color::Black);// ��������� ����� ����������� ����
    mymenu.AlignMenu(2);//������������ ������� ����
    
    //��� ��������� ���
    RectangleShape button(Vector2f(470, 290));
    button.setFillColor(Color::White);
    button.setPosition(100, 90);
    button.setOutlineThickness(1);
    button.setOutlineColor(Color(100, 100, 100));

//��� ���������� �������
    bool menuExpanded = false;//������ ����� ������� �� ���������� �������
    int x = 640;
    int y = 90;
//������ ������ � �������
    RectangleShape for_flovers1(Vector2f(330, 50));//��� ����������� ������ ������
    for_flovers1.setFillColor(Color::White);
    for_flovers1.setOutlineThickness(1);
    for_flovers1.setOutlineColor(Color(100, 100, 100));
    for_flovers1.setPosition(x, y );

    vector<String> name_menu1{ L"+", L"-" }; // �������� ������� ����
    game::MyMenu mymenu1(win, x+305, y, 30, 15, name_menu1);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu1.setColorTextMenu(Color::Black, Color::Black, Color::Black);// ��������� ����� ����������� ����
    mymenu1.AlignMenu(2);//������������ ������� ����

    String f1 = L"1. �������� �������";// �������� ����������� ������
    Text name_flovers1;
    name_flovers1.setFont(font);
    name_flovers1.setFillColor(Color(15,15,15));
    name_flovers1.setString(f1);
    name_flovers1.setCharacterSize(30);
    name_flovers1.setPosition(for_flovers1.getPosition().x + 5, for_flovers1.getPosition().y+ 5);

    RectangleShape for_zifri1(Vector2f(30, 30));//����� ������ ������
    for_zifri1.setFillColor(Color::White);
    for_zifri1.setPosition(for_flovers1.getPosition().x + 260, for_flovers1.getPosition().y + 10);
    for_zifri1.setOutlineThickness(1);
    for_zifri1.setOutlineColor(Color(100, 100, 100));

    int z1 = 0;// ��� �� ������ �����
    Text zifri1;
    zifri1.setFont(font);
    zifri1.setFillColor(Color::Black);
    zifri1.setString(to_string(z1));
    zifri1.setCharacterSize(25);
    zifri1.setOutlineThickness(0.1);
    zifri1.setOutlineColor(Color::Black);
    zifri1.setPosition(for_zifri1.getPosition().x + 2, for_zifri1.getPosition().y);

//������ ������ � �������
    RectangleShape for_flovers2(Vector2f(330, 50));//��� ����������� ������ ������
    for_flovers2.setFillColor(Color::White);
    for_flovers2.setOutlineThickness(1);
    for_flovers2.setOutlineColor(Color(100, 100, 100));
    for_flovers2.setPosition(x , y = y + 50);

    vector<String> name_menu2{ L"+", L"-" }; // �������� ������� ����
    game::MyMenu mymenu2(win, x + 305, y, 30, 15, name_menu2);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu2.setColorTextMenu(Color::Black, Color::Black, Color::Black);// ��������� ����� ����������� ����
    mymenu2.AlignMenu(2);//������������ ������� ����

    String f2 = L"2. ������� �������";// �������� ����������� ������
    Text name_flovers2;
    name_flovers2.setFont(font);
    name_flovers2.setFillColor(Color(15, 15, 15));
    name_flovers2.setString(f2);
    name_flovers2.setCharacterSize(30);
    name_flovers2.setPosition(for_flovers2.getPosition().x + 5, for_flovers2.getPosition().y + 5);

    RectangleShape for_zifri2(Vector2f(30, 30));//����� ������ ������
    for_zifri2.setFillColor(Color::White);
    for_zifri2.setPosition(for_flovers2.getPosition().x + 260, for_flovers2.getPosition().y + 10);
    for_zifri2.setOutlineThickness(1);
    for_zifri2.setOutlineColor(Color(100, 100, 100));

    int z2 = 0;// ��� �� ������ �����
    Text zifri2;
    zifri2.setFont(font);
    zifri2.setFillColor(Color::Black);
    zifri2.setString(to_string(z2));
    zifri2.setCharacterSize(25);
    zifri2.setOutlineThickness(0.1);
    zifri2.setOutlineColor(Color::Black);
    zifri2.setPosition(for_zifri2.getPosition().x + 2, for_zifri2.getPosition().y);


 //3 ������ � �������
    RectangleShape for_flovers3(Vector2f(330, 50));//��� ����������� ������ ������
    for_flovers3.setFillColor(Color::White);
    for_flovers3.setOutlineThickness(1);
    for_flovers3.setOutlineColor(Color(100, 100, 100));
    for_flovers3.setPosition(x, y = y + 50);

    vector<String> name_menu3{ L"+", L"-" }; // �������� ������� ����
    game::MyMenu mymenu3(win, x + 305, y, 30, 15, name_menu3);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu3.setColorTextMenu(Color::Black, Color::Black, Color::Black);// ��������� ����� ����������� ����
    mymenu3.AlignMenu(2);//������������ ������� ����

    String f3 = L"3. ����� ���������";// �������� ����������� ������
    Text name_flovers3;
    name_flovers3.setFont(font);
    name_flovers3.setFillColor(Color(15, 15, 15));
    name_flovers3.setString(f3);
    name_flovers3.setCharacterSize(30);
    name_flovers3.setPosition(for_flovers3.getPosition().x + 5, for_flovers3.getPosition().y + 5);

    RectangleShape for_zifri3(Vector2f(30, 30));//����� ������ ������
    for_zifri3.setFillColor(Color::White);
    for_zifri3.setPosition(for_flovers3.getPosition().x + 260, for_flovers3.getPosition().y + 10);
    for_zifri3.setOutlineThickness(1);
    for_zifri3.setOutlineColor(Color(100, 100, 100));

    int z3 = 0;// ��� �� ������ �����
    Text zifri3;
    zifri3.setFont(font);
    zifri3.setFillColor(Color::Black);
    zifri3.setString(to_string(z3));
    zifri3.setCharacterSize(25);
    zifri3.setOutlineThickness(0.1);
    zifri3.setOutlineColor(Color::Black);
    zifri3.setPosition(for_zifri3.getPosition().x + 2, for_zifri3.getPosition().y);


//4 ������ � �������
    RectangleShape for_flovers4(Vector2f(330, 50));//��� ����������� ������ ������
    for_flovers4.setFillColor(Color::White);
    for_flovers4.setOutlineThickness(1);
    for_flovers4.setOutlineColor(Color(100, 100, 100));
    for_flovers4.setPosition(x, y = y + 50);

    vector<String> name_menu4{ L"+", L"-" }; // �������� ������� ����
    game::MyMenu mymenu4(win, x + 305, y, 30, 15, name_menu4);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu4.setColorTextMenu(Color::Black, Color::Black, Color::Black);// ��������� ����� ����������� ����
    mymenu4.AlignMenu(2);//������������ ������� ����

    String f4 = L"4. �������� �����";// �������� ����������� ������
    Text name_flovers4;
    name_flovers4.setFont(font);
    name_flovers4.setFillColor(Color(15, 15, 15));
    name_flovers4.setString(f4);
    name_flovers4.setCharacterSize(30);
    name_flovers4.setPosition(for_flovers4.getPosition().x + 5, for_flovers4.getPosition().y + 5);

    RectangleShape for_zifri4(Vector2f(30, 30));//����� ������ ������
    for_zifri4.setFillColor(Color::White);
    for_zifri4.setPosition(for_flovers4.getPosition().x + 260, for_flovers4.getPosition().y + 10);
    for_zifri4.setOutlineThickness(1);
    for_zifri4.setOutlineColor(Color(100, 100, 100));

    int z4 = 0;// ��� �� ������ �����
    Text zifri4;
    zifri4.setFont(font);
    zifri4.setFillColor(Color::Black);
    zifri4.setString(to_string(z4));
    zifri4.setCharacterSize(25);
    zifri4.setOutlineThickness(0.1);
    zifri4.setOutlineColor(Color::Black);
    zifri4.setPosition(for_zifri4.getPosition().x + 2, for_zifri4.getPosition().y);

//5 ������ � �������
    RectangleShape for_flovers5(Vector2f(330, 50));//��� ����������� ������ ������
    for_flovers5.setFillColor(Color::White);
    for_flovers5.setOutlineThickness(1);
    for_flovers5.setOutlineColor(Color(100, 100, 100));
    for_flovers5.setPosition(x, y = y + 50);

    vector<String> name_menu5{ L"+", L"-" }; // �������� ������� ����
    game::MyMenu mymenu5(win, x + 305, y, 30, 15, name_menu5);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu5.setColorTextMenu(Color::Black, Color::Black, Color::Black);// ��������� ����� ����������� ����
    mymenu5.AlignMenu(2);//������������ ������� ����

    String f5 = L"5. P��� ���������";// �������� ����������� ������
    Text name_flovers5;
    name_flovers5.setFont(font);
    name_flovers5.setFillColor(Color(15, 15, 15));
    name_flovers5.setString(f5);
    name_flovers5.setCharacterSize(30);
    name_flovers5.setPosition(for_flovers5.getPosition().x + 5, for_flovers5.getPosition().y + 5);

    RectangleShape for_zifri5(Vector2f(30, 30));//����� ������ ������
    for_zifri5.setFillColor(Color::White);
    for_zifri5.setPosition(for_flovers5.getPosition().x + 260, for_flovers5.getPosition().y + 10);
    for_zifri5.setOutlineThickness(1);
    for_zifri5.setOutlineColor(Color(100, 100, 100));

    int z5 = 0;// ��� �� ������ �����
    Text zifri5;
    zifri5.setFont(font);
    zifri5.setFillColor(Color::Black);
    zifri5.setString(to_string(z5));
    zifri5.setCharacterSize(25);
    zifri5.setOutlineThickness(0.1);
    zifri5.setOutlineColor(Color::Black);
    zifri5.setPosition(for_zifri5.getPosition().x + 2, for_zifri5.getPosition().y);


// ��� ���������� ������
    bool menuExpanded2 = false;
    x = 640;
    y = 90;
//������ ������ � ������
    RectangleShape for_vazi1(Vector2f(330, 50));//��� ����������� ������ ������
    for_vazi1.setFillColor(Color::White);
    for_vazi1.setOutlineThickness(1);
    for_vazi1.setOutlineColor(Color(100, 100, 100));
    for_vazi1.setPosition(x, y);

    vector<String> name_menu_vasi1{ L"+", L"-" }; // �������� ������� ����
    game::MyMenu mymenu_vasi1(win, x + 305, y, 30, 15, name_menu_vasi1);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu_vasi1.setColorTextMenu(Color::Black, Color::Black, Color::Black);// ��������� ����� ����������� ����
    mymenu_vasi1.AlignMenu(2);//������������ ������� ����

    String v1 = L"����� ���� (3 ��.)";// �������� ����������� ������
    Text name_vasi1;
    name_vasi1.setFont(font);
    name_vasi1.setFillColor(Color(15, 15, 15));
    name_vasi1.setString(v1);
    name_vasi1.setCharacterSize(30);
    name_vasi1.setPosition(for_vazi1.getPosition().x + 5, for_vazi1.getPosition().y + 5);

    RectangleShape for_vasi1(Vector2f(30, 30));//����� ������ ������
    for_vasi1.setFillColor(Color::White);
    for_vasi1.setPosition(for_vazi1.getPosition().x + 260, for_vazi1.getPosition().y + 10);
    for_vasi1.setOutlineThickness(1);
    for_vasi1.setOutlineColor(Color(100, 100, 100));

    int zv1 = 0;// ��� �� ������ �����
    Text zifri_vazi1;
    zifri_vazi1.setFont(font);
    zifri_vazi1.setFillColor(Color::Black);
    zifri_vazi1.setString(to_string(zv1));
    zifri_vazi1.setCharacterSize(25);
    zifri_vazi1.setOutlineThickness(0.1);
    zifri_vazi1.setOutlineColor(Color::Black);
    zifri_vazi1.setPosition(for_vasi1.getPosition().x + 2, for_vasi1.getPosition().y);

 //2 ������ � ������
    RectangleShape for_vazi2(Vector2f(330, 50));//��� ����������� ������ ������
    for_vazi2.setFillColor(Color::White);
    for_vazi2.setOutlineThickness(1);
    for_vazi2.setOutlineColor(Color(100, 100, 100));
    for_vazi2.setPosition(x, y = y+50);

    vector<String> name_menu_vasi2{ L"+", L"-" }; // �������� ������� ����
    game::MyMenu mymenu_vasi2(win, x + 305, y, 30, 15, name_menu_vasi2);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu_vasi2.setColorTextMenu(Color::Black, Color::Black, Color::Black);// ��������� ����� ����������� ����
    mymenu_vasi2.AlignMenu(2);//������������ ������� ����

    String v2 = L"������� ���� (5 ��.)";// �������� ����������� ������
    Text name_vasi2;
    name_vasi2.setFont(font);
    name_vasi2.setFillColor(Color(15, 15, 15));
    name_vasi2.setString(v2);
    name_vasi2.setCharacterSize(30);
    name_vasi2.setPosition(for_vazi2.getPosition().x + 5, for_vazi2.getPosition().y + 5);

    RectangleShape for_vasi2(Vector2f(30, 30));//����� ������ ������
    for_vasi2.setFillColor(Color::White);
    for_vasi2.setPosition(for_vazi2.getPosition().x + 260, for_vazi2.getPosition().y + 10);
    for_vasi2.setOutlineThickness(1);
    for_vasi2.setOutlineColor(Color(100, 100, 100));

    int zv2 = 0;// ��� �� ������ �����
    Text zifri_vazi2;
    zifri_vazi2.setFont(font);
    zifri_vazi2.setFillColor(Color::Black);
    zifri_vazi2.setString(to_string(zv2));
    zifri_vazi2.setCharacterSize(25);
    zifri_vazi2.setOutlineThickness(0.1);
    zifri_vazi2.setOutlineColor(Color::Black);
    zifri_vazi2.setPosition(for_vasi2.getPosition().x + 2, for_vasi2.getPosition().y);

 //3 ������ � ������
    RectangleShape for_vazi3(Vector2f(330, 50));//��� ����������� ������ ������
    for_vazi3.setFillColor(Color::White);
    for_vazi3.setOutlineThickness(1);
    for_vazi3.setOutlineColor(Color(100, 100, 100));
    for_vazi3.setPosition(x, y= y+50);

    vector<String> name_menu_vasi3{ L"+", L"-" }; // �������� ������� ����
    game::MyMenu mymenu_vasi3(win, x + 305, y, 30, 15, name_menu_vasi3);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu_vasi3.setColorTextMenu(Color::Black, Color::Black, Color::Black);// ��������� ����� ����������� ����
    mymenu_vasi3.AlignMenu(2);//������������ ������� ����

    String v3 = L"������� ���� (10 ��.)";// �������� ����������� ������
    Text name_vasi3;
    name_vasi3.setFont(font);
    name_vasi3.setFillColor(Color(15, 15, 15));
    name_vasi3.setString(v3);
    name_vasi3.setCharacterSize(30);
    name_vasi3.setPosition(for_vazi3.getPosition().x + 5, for_vazi3.getPosition().y + 5);

    RectangleShape for_vasi3(Vector2f(30, 30));//����� ������ ������
    for_vasi3.setFillColor(Color::White);
    for_vasi3.setPosition(for_vazi3.getPosition().x + 260, for_vazi3.getPosition().y + 10);
    for_vasi3.setOutlineThickness(1);
    for_vasi3.setOutlineColor(Color(100, 100, 100));

    int zv3 = 0;// ��� �� ������ �����
    Text zifri_vazi3;
    zifri_vazi3.setFont(font);
    zifri_vazi3.setFillColor(Color::Black);
    zifri_vazi3.setString(to_string(zv3));
    zifri_vazi3.setCharacterSize(25);
    zifri_vazi3.setOutlineThickness(0.1);
    zifri_vazi3.setOutlineColor(Color::Black);
    zifri_vazi3.setPosition(for_vasi3.getPosition().x + 2, for_vasi3.getPosition().y);

//������ � ������������
    bool menuExpanded3 = false;
    x = 640;
    y = 90;
    RectangleShape for_rez1(Vector2f(470, 125));//��� ����������� ������ ������
    for_rez1.setFillColor(Color::White);
    for_rez1.setOutlineThickness(1);
    for_rez1.setOutlineColor(Color(100, 100, 100));
    for_rez1.setPosition(x, y);

    String r1 = L"������������ �� ������������ ������\n�������� �� ��� ��������� � ����:\nresult.txt";// �������� ����������� ������
    Text rez1;
    rez1.setFont(font);
    rez1.setFillColor(Color(15, 15, 15));
    rez1.setString(r1);
    rez1.setCharacterSize(30);
    rez1.setPosition(for_rez1.getPosition().x + 5, for_rez1.getPosition().y + 5);

    Vector2f pos;// ���������� ��������
    Clock clock;// ������
    float time;

    // �������� ����
    while (win.isOpen())
    {
        // ��������� �������
        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == Event::Closed) //��� �������� ����� �������
                win.close();

            if (event.type == Event::KeyReleased)
            {
                // ���������� ������� ��� ��������� ���� ���� (���������� �������, ���������� ������, �����)
                if (event.key.code == Keyboard::Up) { menuExpanded = 0; menuExpanded2 = 0; menuExpanded3 = 0;  mymenu.MoveUp(); }        // �����
                if (event.key.code == Keyboard::Down) { menuExpanded = 0; menuExpanded2 = 0; menuExpanded3 = 0; mymenu.MoveDown(); }    // ����
                if (event.key.code == Keyboard::Return)
                {
                    // ��������� �� ��������� ����� ����
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
                        
                        // �������� ������� ���������� ���������
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
            // ���������� ������� ��� ���� ���������� �������

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.key.code == Mouse::Left)
                {
                    if (menuExpanded)
                    {
                        // �������� ������� ����
                        Vector2f mousePos = win.mapPixelToCoords(Mouse::getPosition(win));
                        int w = 50;
                        //������ ������ � �������
                        if (for_flovers1.getGlobalBounds().contains(mousePos))// ���������, ��� �� ���� ������ ������� ����
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 && mousePos.y <= 115)// ���������, ��� �� ���� �� ������ "+"
                            {
                                z1 = z1 + 1;// ��������� �������� ��� ������ "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 && mousePos.y <= 130)// ���������, ��� �� ���� �� ������ "-"
                            {
                                if (z1 > 0) {// ��������� �������� ��� ������ "-"
                                    z1 = z1 - 1;
                                }
                            }
                            zifri1.setString(to_string(z1));// ��������� ��������� ���� � �����������
                        }
                        //������ ������ � �������
                        if (for_flovers2.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w && mousePos.y <= 115 + w)// ���������, ��� �� ���� �� ������ "+"
                            {
                                z2 = z2 + 1;// ��������� �������� ��� ������ "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w && mousePos.y <= 130 + w)// ���������, ��� �� ���� �� ������ "-"
                            {
                                if (z2 > 0) { // ��������� �������� ��� ������ "-"
                                    z2 = z2 - 1;
                                }
                            }
                            zifri2.setString(to_string(z2));// ��������� ��������� ���� � �����������
                        }
                        //3 ������ � �������
                        if (for_flovers3.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w * 2 && mousePos.y <= 115 + w * 2)// ���������, ��� �� ���� �� ������ "+"
                            {
                                z3 = z3 + 1;// ��������� �������� ��� ������ "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w * 2 && mousePos.y <= 130 + w * 2)// ���������, ��� �� ���� �� ������ "-"
                            {
                                if (z3 > 0) { // ��������� �������� ��� ������ "-"
                                    z3 = z3 - 1;
                                }
                            }
                            zifri3.setString(to_string(z3));// ��������� ��������� ���� � �����������
                        }
                        //4 ������ � �������
                        if (for_flovers4.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w * 3 && mousePos.y <= 115 + w * 3)// ���������, ��� �� ���� �� ������ "+"
                            {
                                z4 = z4 + 1;// ��������� �������� ��� ������ "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w * 3 && mousePos.y <= 130 + w * 3)// ���������, ��� �� ���� �� ������ "-"
                            {
                                if (z4 > 0) { // ��������� �������� ��� ������ "-"
                                    z4 = z4 - 1;
                                }
                            }
                            zifri4.setString(to_string(z4));// ��������� ��������� ���� � �����������
                        }
                        //5 ������ � �������
                        if (for_flovers5.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w * 4 && mousePos.y <= 115 + w * 4)// ���������, ��� �� ���� �� ������ "+"
                            {
                                z5 = z5 + 1;// ��������� �������� ��� ������ "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w * 4 && mousePos.y <= 130 + w * 4)// ���������, ��� �� ���� �� ������ "-"
                            {
                                if (z5 > 0) { // ��������� �������� ��� ������ "-"
                                    z5 = z5 - 1;
                                }
                            }
                            zifri5.setString(to_string(z5));// ��������� ��������� ���� � �����������
                        }
                    }

                    //���������� ������� ��� ���� ���������� ������
                    if (menuExpanded2)
                    {
                        // �������� ������� ����
                        Vector2f mousePos = win.mapPixelToCoords(Mouse::getPosition(win));
                        int w = 50;
                        //������ ������ � �����
                        if (for_vazi1.getGlobalBounds().contains(mousePos))// ���������, ��� �� ���� ������ ������� ����
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 && mousePos.y <= 115)// ���������, ��� �� ���� �� ������ "+"
                            {
                                zv1 = zv1 + 1;// ��������� �������� ��� ������ "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 && mousePos.y <= 130)// ���������, ��� �� ���� �� ������ "-"
                            {
                                if (zv1 > 0) {// ��������� �������� ��� ������ "-"
                                    zv1 = zv1 - 1;
                                }
                            }
                            zifri_vazi1.setString(to_string(zv1));// ��������� ��������� ���� � �����������
                        }
                        //������ ������ � �����
                        if (for_vazi2.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w && mousePos.y <= 115 + w)// ���������, ��� �� ���� �� ������ "+"
                            {
                                zv2 = zv2 + 1;// ��������� �������� ��� ������ "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w && mousePos.y <= 130 + w)// ���������, ��� �� ���� �� ������ "-"
                            {
                                if (zv2 > 0) { // ��������� �������� ��� ������ "-"
                                    zv2 = zv2 - 1;
                                }
                            }
                            zifri_vazi2.setString(to_string(zv2));// ��������� ��������� ���� � �����������
                        }
                        //3 ������ � �����
                        if (for_vazi3.getGlobalBounds().contains(mousePos))
                        {
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 100 + w * 2 && mousePos.y <= 115 + w * 2)// ���������, ��� �� ���� �� ������ "+"
                            {
                                zv3 = zv3 + 1;// ��������� �������� ��� ������ "+"
                            }
                            if (mousePos.x >= 940 && mousePos.x <= 965 && mousePos.y >= 115 + w * 2 && mousePos.y <= 130 + w * 2)// ���������, ��� �� ���� �� ������ "-"
                            {
                                if (zv3 > 0) { // ��������� �������� ��� ������ "-"
                                    zv3 = zv3 - 1;
                                }
                            }
                            zifri_vazi3.setString(to_string(zv3));// ��������� ��������� ���� � �����������
                        }


                    }
                }
            }
        }
        // ���������� �������
        time = clock.getElapsedTime().asMicroseconds();
        time = time / 15000;
        clock.restart();

        // �������� ����
        fon.move(-0.2 * time, 0);
        pos = fon.getPosition();
        if (pos.x < -1280) fon.setPosition(1280, pos.y);
        fon2.move(-0.2 * time, 0);
        pos = fon2.getPosition();
        if (pos.x < -1280) fon2.setPosition(1280, pos.y);

        win.clear();// ������� ������
        win.draw(fon); // ��������� ���������
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
        win.display();// ���������� ������
    }
}

// ��������
void About() {
    
    RenderWindow win(VideoMode(1280, 720), "ARM");// �������� ����
    win.setFramerateLimit(60);
    // �������� ������
    Image icon;
    icon.loadFromFile("Image/icon1.png");
    win.setIcon(1200, 1084, icon.getPixelsPtr());
    
    win.setMouseCursorVisible(false);// ���������� ��������� �������

    // ����� ��� ��� ������
    Image image;
    image.loadFromFile("Image/panel4.jpg");
    image.createMaskFromColor(image.getPixel(0, 0));
    Texture texture;
    texture.loadFromImage(image);
    Sprite panel;
    panel.setTexture(texture);
    panel.setPosition(Vector2f(0, 0));
    panel.setScale(1280 / panel.getLocalBounds().width, 720 / panel.getLocalBounds().height);

    // �������� ����������� ����
    Texture textureFon;
    textureFon.loadFromFile("Image/fon2.jpg");
    RectangleShape fon(Vector2f(1280, 720));
    fon.setTexture(&textureFon);

    RectangleShape fon2(Vector2f(1280, 720));
    fon2.setTexture(&textureFon);
    fon2.setPosition(Vector2f(1280, 0));

    // �������� ������
    Font font;
    font.loadFromFile("font/myFont2.ttf");

    // �������� ������ � ��������� ������
    Text Titul;
    Titul.setFont(font);
    TextFormat Ftext;
    Ftext.size_font = 35;
    Ftext.menu_text_color = Color{ 10, 10, 10 };
    Ftext.bord = 0;
    InitText(Titul, 400, 160, L"�������������� ��������:", Ftext);

    // �������� ������
    Font font2;
    font2.loadFromFile("font/myFont2.ttf");

    // �������� ������ � ��������� ������
    Text Titul2;
    Titul2.setFont(font2);
    TextFormat Ftext2;
    Ftext2.size_font = 4;
    Ftext2.menu_text_color = Color( 20, 20, 20 );
    Ftext2.bord = 0;
    InitText(Titul2, 200, 220, L"\t������ ��������� ������������ �� ���� ������������������ \n������� ����� ��������.\n\n\t��� �������� �������� � �������� ������ �������� �� ����� \n����������� ������� ������.\n\n\t� ��� ������� ����� ����������� ���� �������� ������ � \n�������������� �������������� �������� ���������.", Ftext);

    //������� ����� (����������� ����� ����)
    vector<String> name_menu2{ L"������� Enter" };// �������� ������� ����
    game::MyMenu mymenu(win, 930, 560, 20, 0, name_menu2);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu.setColorTextMenu(Color(20, 20, 20), Color(20, 20, 20), Color(20,20,20)); // ��������� ����� ����������� ����
    mymenu.AlignMenu(3);

    Vector2f pos;// ���������� ��������
    Clock clock;// ������
    float time;

    // �������� ����
    while (win.isOpen())
    {
        // ��������� �������
        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) //�������� ����� �������
                win.close();

            if (event.type == Event::KeyReleased)
            {
                // ������� ����� ������� ����
                if (event.key.code == Keyboard::Up) { mymenu.MoveUp(); }        // �����
                if (event.key.code == Keyboard::Down) { mymenu.MoveDown(); }    // ����
                if (event.key.code == Keyboard::Return)                         // ����
                {
                    // ��������� �� ��������� ����� ����
                    switch (mymenu.getSelectedMenuNumber())
                    {
                    case 0:win.close(); break;
                    default:break;
                    }
                }
            }
        }
        // ���������� ������� 
        time = clock.getElapsedTime().asMicroseconds();
        time = time / 8000;
        clock.restart();

        // �������� ����
        fon.move(-0.2 * time, 0);
        pos = fon.getPosition();
        if (pos.x < -1280) fon.setPosition(1280, pos.y);

        fon2.move(-0.2 * time, 0);
        pos = fon2.getPosition();
        if (pos.x < -1280) fon2.setPosition(1280, pos.y);

        win.clear();// ������� ������
        win.draw(fon);// ��������� ���������
        win.draw(fon2);
        win.draw(panel);
        mymenu.draw();
        win.draw(Titul);
        win.draw(Titul2);
        win.display();// ���������� ������
    }
}