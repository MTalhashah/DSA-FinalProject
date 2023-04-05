#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <sstream>
#include <fstream>
#pragma warning(disable : 4996)
#pragma warning(disable : 4244)
using namespace std;
using namespace std::chrono;
//puzzle pool
string bubbleSort(int arr[], int size)//puzzle1
{
    int i, j;
    bool swapped;
    for (i = 0; i < size - 1; i++)
    {
        swapped = false;
        for (j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j + 1], arr[j]);
                swapped = true;
            }
        }
        if (swapped == false)//outer loop breaks if there isn't a single swap in inner loop.
            break;
    }

    string str, s;
    for (int i = 0; i < size; i++)
    {
        stringstream o1;
        o1 << arr[i];
        o1 >> s;
        str += s;
    }
    
    return str;
}
string selectionSort(int arr[], int size)//puzzle2
{
    int min = 0;
    for (int i = 0; i < size - 1; i++)
    {
        min = i;
        for (int j = i + 1; j < size; j++)
        {
            if (arr[j] < arr[min])
            {
                swap(arr[min], arr[j]);
            }
        }
    }

    string str, s;
    for (int i = 0; i < size; i++)
    {
        stringstream o1;
        o1 << arr[i];
        o1 >> s;
        str += s;
    }

    return str;
}
string insertionSort(int arr[], int size)//puzzle3
{
    int i, j;
    for (i = 0; i < size; i++)
    {
        j = i;
        while (j > 0)
        {
            if (arr[j] < arr[j - 1])
            {
                swap(arr[j - 1], arr[j]);
            }
            else
            {
                break;
            }
            j--;
        }
    }

    string str, s;
    for (int i = 0; i < size; i++)
    {
        stringstream o1;
        o1 << arr[i];
        o1 >> s;
        str += s;
    }
    
    return str;
}
//puzzle pool ends here

//function chosing puzzle from pool
string win[3];
string winArr[3];
int tcode = 0;
int d[3];
void pool(string x)
{
    //generating random array
    srand((unsigned int)time(NULL));
    int ran = rand() % 8;
    int arr[1000], j = 0, size = 1000;
    for (int i = size-1; i >= 0; i--)
    {
        j++;
        arr[i] = j * ran;
    }

    //choosing random puzzle and solving it
    string sorted;
    auto start = high_resolution_clock::now();
    if (ran <= 2)
    {
        sorted = bubbleSort(arr, size);
    }
    else if (ran >= 3 && ran <= 5)
    {
        sorted = selectionSort(arr, size);
    }
    else if (ran >= 6)
    {
        sorted = insertionSort(arr, size);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    d[tcode] = duration.count();
    win[tcode] = x;
    winArr[tcode] = sorted;
    tcode++;
}

//generating hash
int hashFunction(string key)
{
    int n = key.length();
    int hashCode = 0;
    for (int i = 0; i < n; i++)
    {
        hashCode += key[i]  * (i+1);
    }
    return hashCode;
}

//filing
void filing(int s, int t, string fileName)
{
    time_t now = time(NULL);
    char* dt = ctime(&now);
    int hashed = hashFunction(winArr[0]);

    ofstream file(fileName, std::ios_base::app);
    file << "Data: " << s << endl;
    file << "Hash: " << hashed << endl;
    file << "Execution Time: " << t << " MicroSeconds" << endl;
    file << "Time stamp: " << dt << endl;
    file.close();

    ofstream file1("Log File.txt", std::ios_base::app);
    file1 << "Data: " << s << endl;
    file1 << "Hash: " << hashed << endl;
    file1 << "Execution Time: " << t << " MicroSeconds" << endl;
    file1 << "Time stamp: " << dt << endl;
    file1.close();
}

int main()
{
    int reward;
    //if giving specific reward to miner
    cout << "Enter Reward: ";
    cin >> reward;
    //if giving random reward to user
    //srand(time(NULL));
    //reward = rand();

    //tcode = 0;
    thread miner1(pool, "1");
    thread miner2(pool, "2");
    thread miner3(pool, "3");

    miner1.join();
    miner2.join();
    miner3.join();

    if (win[0] == "1" && winArr[0] == winArr[1] && winArr[0] == winArr[2])
    {
        filing(reward, d[0], "Thread 1_Reward.txt");
        cout << "Thread 1 wins.";
    }
    else if (win[0] == "2" && winArr[1] == winArr[0] && winArr[1] == winArr[2])
    {
        filing(reward, d[0], "Thread 2_Reward.txt");
        cout << "Thread 2 wins.";
    }
    else if (win[0] == "3" && winArr[2] == winArr[1] && winArr[2] == winArr[0])
    {
        filing(reward, d[0], "Thread 3_Reward.txt");
        cout << "Thread 3 wins.";
    }
}