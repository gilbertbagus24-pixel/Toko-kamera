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


void addProduct() {
    Product p;

    cout << "\nID Produk   : ";
    cin >> p.id;
    cin.ignore();

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

    Product p;
    char comma;

    while(file >> p.id >> comma) {
        getline(file, p.name, ',');
        file >> p.price >> comma >> p.stock;
        products.push_back(p);
    }

    file.close();
}

int main() {
    loadData();

    int choice;

    do {
        cout << "\n===== KREATIV STORE =====\n";
        cout << "1. Tambah Produk\n";
        cout << "2. Tampilkan Produk\n";
        cout << "3. Sewa Produk\n";
        cout << "4. Simpan Data\n";
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        cin >> choice;

        switch(choice) {
            case 1: addProduct(); break;
            case 2: showProducts(); break;
            case 3: rentProduct(); break;
            case 4: saveData(); break;
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