#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>

using namespace std;

const string fileName = "dump.txt"; //Сериализовать будем в текстовый файл

class ListNode
{
public:
    ListNode *Prev;
    ListNode *Next;
    ListNode *Rand; // произвольный элемент внутри списка
    string Data;
};


class ListRand
{

public:
    ListNode* Head;
    ListNode* Tail;
    int Count;

public:
    void Serialize(ofstream &s)
    {
        s << Count << endl; // В первую строку файла записываем Count
        
        int *randIndex = new int[Count];
        vector<ListNode *> listNodesVector;
        queue<ListNode *> newQueue;

        newQueue.push(Head);

        while (!newQueue.empty())
        {
            ListNode* nextNode = newQueue.back();
            newQueue.pop();

            if (nextNode)
            {
                listNodesVector.push_back(nextNode);
                newQueue.push(nextNode->Next);
            }
        }

        for (int i = 0; i < listNodesVector.size(); i++)
        {
            // Записываем все элементы списка построчно в файл
            s << listNodesVector[i]->Data << endl; // Запись самой даты
            
            randIndex[i] = listNodesVector[i]->Rand ? 
                                    listNodesVector[i]->Rand - listNodesVector[0] : -1;

            s << randIndex[i] << endl; // Запись индекса рандомного элемента
        }

        delete[] randIndex;
    }

    void Deserialize(ifstream &s)
    {
        vector<ListNode *> listNodeVector;

        s >> Count; // Считываем Count 
        
        int *randIndex = new int[Count];

        for (int i = 0; i < Count; i++)
        {
            ListNode* newNode = new ListNode();
            
            // Записываем дату в элементы
            s >> newNode->Data;
            s >> randIndex[i]; // Считываем индексы
            
            listNodeVector.push_back(newNode);
        }

        for (int i = 0; i < Count; i++)
        {
            // Востанавливаем связи элементов
            if (i == 0)
            {
                Head = listNodeVector[i];
                listNodeVector[i]->Next = listNodeVector[i + 1];
                listNodeVector[i]->Prev = nullptr;
            }
            else if (i == Count - 1)
            {
                Tail = listNodeVector[i];
                listNodeVector[i]->Prev = listNodeVector[i - 1];
                listNodeVector[i]->Next = nullptr;
            }
            else
            {
                listNodeVector[i]->Next = listNodeVector[i + 1];
                listNodeVector[i]->Prev = listNodeVector[i - 1];
            }

            // Востанавливаем связи элементов с рандомными
            if (randIndex[i] != -1)
            {
                listNodeVector[i]->Rand = listNodeVector[randIndex[i]];
            }
            else
            {
                listNodeVector[i]->Rand = nullptr;
            }
        }

        delete[] randIndex;

        print(); // Печатает получившиеся значения
    }

    void print() // Метод исключительно для проверки, никаких изменений в класс он не вносит
    {
        queue<ListNode*> newQueue;

        newQueue.push(Head);

        while (!newQueue.empty())
        {
            ListNode* nextNode = newQueue.back();
            newQueue.pop();

            if (nextNode)
            {
                cout << nextNode->Data << endl;
                if (nextNode->Rand)
                {
                    cout << "Элемент выше указывает на рандомный элемент со значением: ";
                    if (nextNode->Rand)
                    {
                        cout << nextNode->Rand->Data << endl;
                    }
                }
   
                newQueue.push(nextNode->Next);
            }
        }   
    }
};


int main()
{
    //  Код ниже нужен только для тестов, и никак не связан с алгоритмами
    vector<ListNode> nodesVector;

    // Создаём список
    for (int i = 0; i < 5; i++)
    {
        ListNode newNode;
        newNode.Data = to_string(i) + "wwerwerwer";
        nodesVector.push_back(newNode);
    }

    // Соединяем данные (захардкодил соединения рандомных элементов из примера)
    for (int i = 0; i < nodesVector.size(); i++)
    {
        if (i == 0)
        {
            nodesVector[i].Next = &nodesVector[i + 1];
            nodesVector[i].Prev = nullptr;
        }
        else if (i == nodesVector.size() - 1)
        {
            nodesVector[i].Next = nullptr;
            nodesVector[i].Prev = &nodesVector[i - 1];
        }
        else
        {
            nodesVector[i].Next = &nodesVector[i + 1];
            nodesVector[i].Prev = &nodesVector[i - 1];
        }

        if (i == 1)
        {
            nodesVector[i].Rand = &nodesVector[i];
        }
        else if (i == 2)
        {
            nodesVector[i].Rand = &nodesVector[4];
        }
        else if (i == 3)
        {
            nodesVector[i].Rand = &nodesVector[0];
        }
        else
        {
            nodesVector[i].Rand = nullptr;
        }

    }
    
    ListRand firstRand;
    ListRand secondRand;

    firstRand.Head = &nodesVector[0];
    firstRand.Tail = &nodesVector[nodesVector.size() - 1];
    firstRand.Count = nodesVector.size();
    // Код для теста закончен

    // Настравиваем fstream чтобы он писал байты
    ofstream s(fileName, ios::binary);
    ifstream n(fileName);

    firstRand.Serialize(s); // Сериализуем файлы
    secondRand.Deserialize(n); // Десериализуем файлы
    // Десериализация работает на файлах которые были сериализованы на разных компьютерах
    // Для проверки закомментите строки где есть переменная s т.е. 212, 215, 220

    s.close();
    n.close();

    return 0;
}

