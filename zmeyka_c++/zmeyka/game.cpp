#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

bool gameOver;                     //глобальная переменная отвечающая за состояние проигрыша
const int width = 20;              //значение ширины поля с 0..19 (=20)
const int height = 20;             //и высоты поля
int x, y;                          //координаты змейки
int fruitX, fruitY;                //координаты фруктов
int score;                         //счёт игры
int tailX[100], tailY[100];        //массивы, координаты хвоста
int nTail;                         //количество элементов в хвосте
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };   //перечисления управления
eDirection dir;

void Setup()                       //функция отвечает за установку параметров игры
{
    gameOver = false;              //переменная проигрыша изначально в состоянии 0
    dir = STOP;                    //изначально змейка стоит на месте
    x = width / 2 - 1;
    y = height / 2 - 1;             //положение змейки относительно карты по-центру
    fruitX = rand() % width;       //положение рандомной горизонтальной координаты для фрукта
    fruitY = rand() % height;      //рандом фрукта по вертикали
    score = 0;
}

void Draw()                       //функция отрисовки карты
{
    Sleep(130);                            //скорость игры
    system("cls");                        //такой костыль типо обновление карты(отрисовка) при каждом вызове
    for(int i = 0; i < width + 1; i++)    //верхняя граница карты(width= 0..20, height=0)
    {
        cout << "#";
    }
    cout << endl;

    for(int i = 0; i < height; i++)       //этот цикл отвечает за новую строку, по окончании вложенного переводит на новую строку
    {
        for(int j = 0; j < width; j++)    //этот в 0, 19 отрисовывает '#' | с 1..18 - " " и отступив строку возвр. к ↑ циклу
        {
            if (j == 0 || j == width -1)  //в 1-ой и последней ячейке отрисовывается # (-1 тк отсчёт идет с 0)
            {
                cout << "#";              //отрисовывается # когда j= 0, 19 (height= 1..18 -- область действия)
            }
            if ( i == y && j == x )       //проверка равна ли в даанный момент i= 20/2 или j= 20/2 ? если нет к elseif
            {
                cout << "0";              //отобразить змейку в соответсвии с текущем ее положением
            }
            else if ( i == fruitY && j == fruitX)
            {
                cout << "*";
            }
            else {
                    bool print = false;                 //если съели фрукт прибавляем хвост, если нет пробел
                    for (int k =0; k < nTail; k++)
                    {
                        if (tailX[k] == j && tailY[k] == i)
                        {
                            print = true;
                            cout << "o";
                        }
                    }
                if (!print)
                    cout << " ";                 //пробелом будут заполнены все остальные ячейки по-ширине j=1..18 (height= 1..18)
        }
     }
        cout << endl;
   }
    for(int i = 0; i < width + 1; i++)        //нижняя граница карты(width= 0..19, height=19)
    {
        cout << "#";
    }
    cout << endl;
    cout << "\npoints: " << score << endl;  //вывод очков от границы нижней грани
}

void Input()                      //нажатие клавиш пользователем
{
    if(_kbhit())                  //функция для отслеживания нажатия клавиш пользователем
    {
        switch (_getch())
        {
            case 'a':            //если а то будет двигаться влево
                dir = LEFT;
                break;
            case 'd':            //если d то будет двигаться вправо
                dir = RIGHT;
                break;
            case 'w':            //если w то будет двигаться вверх
                dir = UP;
                break;
            case 's':            //если s то будет двигаться вниз
                dir = DOWN;
                break;
            case 'x':            //если х то выход из игры
                gameOver = true;
                break;
        }
    }
}

void Logic()                      //функция логики игры
{
    int prevX = tailX[0];         //при начале игры координаты хвоста
    int prevY = tailY[0];
    int prev2X, prev2Y;           //переменные в которые мы закладываем доп. секции
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir)                  //управление змейкой в соответствии с координатами
    {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
    }

    //if (x > width || x < 0 || y > height || y < 0)   // если змейка выходит за пределы поля игра завершается
    //{
    //    gameOver = true;
    //}

    if (x >= width -1)                                // если змейка выходит за правую грань
    {
        x = 0;                                     //то выходит за левой
    }
    else if (x < 0)                                //если выходит за левую грань
    {
        x = width - 2;                             //то появляется справа
    }

    if (y >= height)                                // если змейка выходит за низ
    {
        y = 0;                                     //то выходит за сверху
    }
    else if (y < 0)                                //если выходит за потолок
    {
        y = height - 1;                             //то появляется снизу
    }

    for (int i = 0; i < nTail; i++)                 //проверка на съедение хвоста
    {
        if(tailX[i] == x && tailY[i] == y)          //если координаты головы совпадают с координатами хвоста то игра завершается
            gameOver = true;
    }

    if (x == fruitX && y == fruitY)                //если лопаем фрукт + 10 очков
    {
       score += 10;
       fruitX = rand() % width;                    //новые координаты после поедания
       fruitY = rand() % height;
       nTail++;                                    //каждый раз хвостик растет на 1
    }
}

int main()                        //main используем как катализатор запуска и вызова функций игры
{
    Setup();                      //устанавливаем настройки при запуске игры
    while (!gameOver)             //пока игра активна запускаем функции ниже..
    {
      Draw();
      Input();
      Logic();
    }

	return 0;
}
