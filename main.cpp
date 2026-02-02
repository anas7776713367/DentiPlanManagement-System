#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>

using namespace std;

/**
 * دالة لتغيير لون النص في نافذة الكونسول
 * Function to change text color in Windows Console
 */
void setcolor(int color) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, (WORD)color);
}

/**
 * هيكل يمثل "العقدة" - تمثل بيانات مريض واحد في القائمة
 * Node Structure - Represents a single patient in the list
 */
struct Node {
    string time;   // وقت الموعد
    string name;   // اسم المريض
    string phone;  // رقم الهاتف
    Node* next;    // مؤشر للعقدة التالية

    // Constructor لتجهيز بيانات العقدة الجديدة
    Node(string name, string phone, string time) 
        : name(name), phone(phone), time(time), next(NULL) {}
};

/**
 * هيكل يمثل "طابور الانتظار" باستخدام Linked List
 * Queue implementation for the Waiting List
 */
struct Queue_list {
    Node *front, *rear; // مؤشرات لبداية ونهاية الطابور

    Queue_list() { front = rear = NULL; }

    // إضافة مريض إلى نهاية قائمة الانتظار (Enqueue)
    void add_waiting(string name, string phone, string time) {
        Node* newNode = new Node(name, phone, time);
        if (front == NULL) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    // عرض جميع المرضى في قائمة الانتظار
    void display_Waiting() {
        cout << "\t\t\tWAITING LIST\n";
        Node* curr = front;
        if (front == NULL) {
            cout << "Wait list is empty.\n";
            return;
        }
        int count = 1;
        while (curr != NULL) {
            cout << "[" << count << "] Name: " << curr->name << " | Time: " << curr->time << "\n";
            curr = curr->next;
            count++;
        }
    }

    // استخراج أول مريض من قائمة الانتظار (Dequeue)
    Node* deQueue() {
        if (front == NULL) return NULL;
        Node* temp = front;
        front = front->next;
        if (front == NULL) rear = NULL;
        temp->next = NULL;
        return temp;
    }
};

/**
 * هيكل يمثل "جدول المواعيد المحجوزة"
 * Linked List for handling fixed Appointments
 */
struct BookingList {
    Node* head;

    BookingList() { head = NULL; }

    // التحقق مما إذا كان الوقت محجوزاً مسبقاً
    bool isTimeTaken(const string& time) const {
        Node* curr = head;
        while (curr != NULL) {
            if (curr->time == time) return true;
            curr = curr->next;
        }
        return false;
    }

    // إضافة موعد جديد أو نقله للانتظار إذا كان الوقت مشغولاً
    void addAppointment(string name, string phone, string time, Queue_list& waiting) {
        if (isTimeTaken(time)) {
            cout << "!! Time Slot Busy. Moving to Waiting List...\n";
            waiting.add_waiting(name, phone, time);
            return;
        }
        Node* newNode = new Node(name, phone, time);
        // إضافة في بداية القائمة إذا كانت فارغة أو التوقيت أصغر
        if (head == NULL) {
            head = newNode;
        } else {
            Node* curr = head;
            while (curr->next != NULL) curr = curr->next;
            curr->next = newNode;
        }
        cout << "Success: Appointment Booked.\n";
    }

    // البحث عن موعد باستخدام الاسم
    void search_Appointment(string key) {
        Node* curr = head;
        bool found = false;
        while (curr != NULL) {
            if (curr->name == key) {
                cout << ">> Found: " << curr->name << " at " << curr->time << "\n";
                found = true;
                break;
            }
            curr = curr->next;
        }
        if (!found) cout << "Name not found.\n";
    }

    // إلغاء موعد وسحب مريض من الانتظار لملء الفراغ
    void cancel_Appointment(string time, Queue_list& queue) {
        Node *curr = head, *prev = NULL;
        while (curr != NULL && curr->time != time) {
            prev = curr;
            curr = curr->next;
        }
        if (curr == NULL) {
            cout << "No appointment at this time.\n";
            return;
        }
        // حذف العقدة من القائمة المرتبطة
        if (prev == NULL) head = curr->next;
        else prev->next = curr->next;
        delete curr;

        cout << "Appointment Canceled.\n";
        
        // منطق "ملء الفراغ": سحب أول شخص من قائمة الانتظار
        Node* nextPatient = queue.deQueue();
        if (nextPatient != NULL) {
            cout << "Automatic Booking for Next Patient from Waiting List...\n";
            addAppointment(nextPatient->name, nextPatient->phone, time, queue);
            delete nextPatient;
        }
    }
};

int main() {
    BookingList bookings;      // كائن لإدارة المواعيد المؤكدة
    Queue_list waitingQueue;   // كائن لإدارة قائمة الانتظار
    int choice;

    // الحلقة اللانهائية لعرض القائمة الرئيسية
    while (true) {
        setcolor(11); // لون تركواز
        cout << "\n--- DENTAL CLINIC SYSTEM ---\n";
        setcolor(7);  // العودة للأبيض
        cout << "1. Add Appointment\n4. Search\n5. View All\n6. View Waiting\n10. Exit\n";
        cout << "Choice: ";
        
        if (!(cin >> choice)) { // التحقق من إدخال رقم وليس نص
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        string name, phone, time;
        switch (choice) {
            case 1:
                cout << "Name: "; cin.ignore(); getline(cin, name);
                cout << "Phone: "; cin >> phone;
                cout << "Time (e.g. 10:30): "; cin >> time;
                bookings.addAppointment(name, phone, time, waitingQueue);
                break;
            case 5:
                bookings.display_Appointments(); // (يجب استدعاء دالة العرض)
                break;
            case 10:
                return 0; // خروج
        }
    }
    return 0;
}