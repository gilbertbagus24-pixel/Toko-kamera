#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Product {
    int id;
    string name;
    double price;
    int stock;
};

vector<Product> products;

void saveData() {
    ofstream file("data.txt");
    for(auto p : products) {
        file << p.id << ","
             << p.name << ","
             << p.price << ","
             << p.stock << endl;
    }
    file.close();
    cout << "Data berhasil disimpan!\n";
}

void loadData() {
    ifstream file("data.txt");
    if(!file.is_open())
        return;

    products.clear();
    Product p;
    char comma;
    while(file >> p.id >> comma) {
        getline(file, p.name, ',');
        file >> p.price >> comma >> p.stock;
        products.push_back(p);
    }
    file.close();
}

void addProduct() {
    Product p;

    cout << "\nID Produk   : ";
    cin >> p.id;
    cin.ignore();

    for(auto existing : products) {
        if(existing.id == p.id) {
            cout << "ID sudah terdaftar!\n";
            return;
        }
    }

    cout << "Nama Produk : ";
    getline(cin, p.name);

    cout << "Harga       : ";
    cin >> p.price;

    cout << "Stock       : ";
    cin >> p.stock;

    products.push_back(p);
    cout << "Produk berhasil ditambahkan!\n";
}

void showProducts() {
    cout << "\n===== LIST PRODUK =====\n";

    if(products.empty()) {
        cout << "Belum ada produk.\n";
        return;
    }

    for(auto p : products) {
        cout << p.id << " | "
             << p.name << " | Rp"
             << p.price << " | Stock: "
             << p.stock << endl;
    }
}

void insertionSort() {
    int n = products.size();
    for (int i = 1; i < n; i++) {
        Product key = products[i];
        int j = i - 1;

        while (j >= 0 && products[j].price > key.price) {
            products[j + 1] = products[j];
            j = j - 1;
        }
        products[j + 1] = key;
    }
    cout << "Produk berhasil diurutkan berdasarkan harga termurah!\n";
}

void rentProduct() {
    int id, qty;
    bool found = false;

    cout << "\nMasukkan ID produk: ";
    cin >> id;

    for(auto &p : products) {
        if(p.id == id) {
            found = true;

            cout << "Jumlah sewa: ";
            cin >> qty;

            if(qty > p.stock) {
                cout << "Stock tidak cukup!\n";
                return;
            }

            p.stock -= qty;

            double total = qty * p.price;
            cout << "Berhasil disewa!\n";
            cout << "Total bayar: Rp" << total << endl;
            return;
        }
    }

    if(!found)
        cout << "Produk tidak ditemukan!\n";
}

bool loginAdmin() {
    string username, password;
    cout << "\nUsername: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username == "admin" && password == "admin123") {
        cout << "Login Berhasil!\n";
        return true;
    } else {
        cout << "Login Gagal!\n";
        return false;
    }
}

void adminMenu() {
    int choice;
    do {
        cout << "\n===== MENU ADMIN =====\n";
        cout << "1. Tambah Produk\n";
        cout << "2. Tampilkan Produk\n";
        cout << "3. Urutkan Produk (Harga)\n";
        cout << "4. Simpan Data\n";
        cout << "0. Log Out\n";
        cout << "Pilih: ";
        cin >> choice;

        switch(choice) {
            case 1: addProduct(); break;
            case 2: showProducts(); break;
            case 3: insertionSort(); break;
            case 4: saveData(); break;
            case 0: break;
            default: cout << "Pilihan salah!\n";
        }
    } while(choice != 0);
}

void buyerMenu() {
    int choice;
    do {
        cout << "\n===== MENU PEMBELI =====\n";
        cout << "1. Tampilkan Produk\n";
        cout << "2. Urutkan Produk (Harga)\n";
        cout << "3. Sewa Produk\n";
        cout << "0. Kembali\n";
        cout << "Pilih: ";
        cin >> choice;

        switch(choice) {
            case 1: showProducts(); break;
            case 2: insertionSort(); break;
            case 3: rentProduct(); break;
            case 0: break;
            default: cout << "Pilihan salah!\n";
        }
    } while(choice != 0);
}

int main() {
    loadData();

    int choice;
    do {
        cout << "\n===== KREATIV STORE =====\n";
        cout << "Masuk Sebagai:\n";
        cout << "1. Admin\n";
        cout << "2. Pembeli\n";
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        cin >> choice;

        switch(choice) {
            case 1:
                if (loginAdmin()) {
                    adminMenu();
                }
                break;
            case 2:
                buyerMenu();
                break;
            case 0:
                saveData();
                cout << "Program selesai.\n";
                break;
            default:
                cout << "Pilihan salah!\n";
        }
    } while(choice != 0);

    return 0;
}