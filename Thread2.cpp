// Thread2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <windows.h>

std::mutex mu; //digunakan untuk melindungi data bersama agar tidak diakses secara bersamaan oleh banyak thread.
std::condition_variable cond; //digunakan untuk memblokir thread atau multiple thread secara bersamaan, hingga thread lain mengubah variabel bersama (kondisi), dan memberi tahu condition_variable.
int count = 1;

//fungsi ini untuk mengatur timer antar thread
void timer(int sec) {
    Sleep(sec * 1000);
}

//fungsi untuk mencetak angka ganjil
void PrintOdd()
{
    for (; count < 10;)     //banyaknya bilangan yang akan dicetak
    {
        std::unique_lock<std::mutex> locker(mu);        //bilangan akan dikunci pada thread ini untuk memblokir akses thread lain
        cond.wait(locker, []() { return (count % 2 == 1); });       //menunggu kondisi bilangan yang jika dibagi 2 ada sisa dan kemudian akan dikembalikan (diprint)
        timer(1);       //memberi timer atau delay pada bilangan yang akan dicetak
        std::cout << "Thread 1 Odd :        " << count << std::endl;         //cetak bilangan ganjil
        count++;        //bilangan terus berjalan menuju bilangan selanjutnya
        locker.unlock();    //membuka kunci dari thread ini agar bilangan yang bukan ganjil dapat diakses di thread lain
        cond.notify_all();  //membuka blokir semua thread yang sedang menunggu kondisi ini
    }
}

//fungsi untuk mencetak angka genap
void PrintEven()
{
    for (; count < 10;)
    {
        std::unique_lock<std::mutex> locker(mu);         //bilangan akan dikunci pada thread ini untuk memblokir akses thread lain
        cond.wait(locker, []() { return (count % 2 == 0); });       //menunggu kondisi bilangan yang jika dibagi 2 = 0 dan kemudian akan dikembalikan (diprint)
        timer(3);       //memberi timer atau delay pada bilangan yang akan dicetak
        std::cout << "Thread 2 Even :   " << count << std::endl;           //cetak bilangan ganjil
        count++;            //bilangan terus berjalan menuju bilangan selanjutnya
        locker.unlock();        //membuka kunci dari thread ini agar bilangan yang bukan genap dapat diakses di thread lain
        cond.notify_all();      //membuka blokir semua thread yang sedang menunggu kondisi ini
    }
}


int main()
{
    //panggil thread 
    std::thread t1(PrintOdd);
    std::thread t2(PrintEven);
    
    //Fungsi return ketika eksekusi thread telah selesai.
    t1.join();
    t2.join();
    return 0;
}