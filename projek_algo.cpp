#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
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

void insertionSort() {
    int n = (int)products.size();
    for (int i = 1; i < n; i++) {
        Product key = products[(size_t)i];
        int j = i - 1;

        while (j >= 0 && products[(size_t)j].price > key.price) {
            products[(size_t)(j + 1)] = products[(size_t)j];
            j = j - 1;
        }
        products[(size_t)(j + 1)] = key;
    }
    cout << "Produk berhasil diurutkan berdasarkan harga termurah!\n";
}

void showProducts(bool isBuyer = false) {
    char stayChoice;
    do {
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

        char sortChoice;
        cout << "\nUrutkan berdasarkan harga termurah? (y/n): ";
        cin >> sortChoice;

        if(sortChoice == 'y' || sortChoice == 'Y') {
            insertionSort();
            cout << "\n===== LIST PRODUK (TERURUT) =====\n";
            for(auto p : products) {
                cout << p.id << " | "
                     << p.name << " | Rp"
                     << p.price << " | Stock: "
                     << p.stock << endl;
            }
        }

        if (isBuyer) {
            cout << "\nApakah ingin tetap di halaman list produk? (y/n): ";
            cin >> stayChoice;
        } else {
            stayChoice = 'n';
        }
    } while(stayChoice == 'y' || stayChoice == 'Y');
}

void rentProduct() {
    int id, qty;
    bool found = false;

    cout << "\n=== TRANSAKSI SEWA PRODUK ===\n";
    cout << "Masukkan ID produk yang ingin disewa: ";
    cin >> id;

    for(auto &p : products) {
        if(p.id == id) {
            found = true;

            cout << "\n--- DETAIL PRODUK (DATABASE) ---\n";
            cout << "ID           : " << p.id << endl;
            cout << "Nama Produk  : " << p.name << endl;
            cout << "Harga Sewa   : Rp" << p.price << " / unit" << endl;
            cout << "Stok Tersedia: " << p.stock << endl;
            cout << "--------------------------------\n";

            if(p.stock <= 0) {
                cout << "Maaf, stok produk saat ini sedang habis!\n";
                return;
            }

            cout << "\nMasukkan jumlah sewa: ";
            cin >> qty;

            if(qty <= 0) {
                cout << "Jumlah sewa tidak valid!\n";
                return;
            }

            if(qty > p.stock) {
                cout << "Stok tidak mencukupi!\n";
                return;
            }

            p.stock -= qty;

            double total = qty * p.price;
            cout << "\n[TRANSAKSI BERHASIL]\n";
            cout << "Produk      : " << p.name << endl;
            cout << "Jumlah Sewa : " << qty << " unit" << endl;
            cout << "Total Bayar : Rp" << total << endl;
            cout << "---------------------\n";

            saveData(); // Otomatis simpan perubahan stok ke database file
            return;
        }
    }

    if(!found)
        cout << "Produk tidak ditemukan di database!\n";
}

bool loginAdmin() {
    string username, password;
    cout << "\nUsername: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username == "admin" && password == "admin123") {
        cout << "Login Berhasil!\n";
        system("pause");
        return true;
    } else {
        cout << "Login Gagal!\n";
        system("pause");
        return false;
    }
}

void adminMenu() {
    int choice;
    do {
        system("cls");
        cout << "\n===== MENU ADMIN =====\n";
        cout << "1. Tambah Produk\n";
        cout << "2. Tampilkan Produk\n";
        cout << "3. Simpan Data\n";
        cout << "4. Ambil Data\n";
        cout << "0. Log Out\n";
        cout << "Pilih: ";
        cin >> choice;

        switch(choice) {
            case 1: 
                addProduct(); 
                system("pause");
                break;
            case 2: 
                showProducts(); 
                system("pause");
                break;
            case 3: 
                saveData(); 
                system("pause");
                break;
            case 4:
                loadData();
                cout << "Data berhasil diambil!\n";
                system("pause");
                break;
            case 0: break;
            default: 
                cout << "Pilihan salah!\n";
                system("pause");
        }
    } while(choice != 0);
}

void buyerMenu() {
    int choice;
    do {
        system("cls");
        cout << "\n===== MENU PEMBELI =====\n";
        cout << "1. Tampilkan Produk\n";
        cout << "2. Sewa Produk\n";
        cout << "0. Kembali\n";
        cout << "Pilih: ";
        cin >> choice;

        switch(choice) {
            case 1: 
                showProducts(true); 
                system("pause");
                break;
            case 2: 
                rentProduct(); 
                system("pause");
                break;
            case 0: break;
            default: 
                cout << "Pilihan salah!\n";
                system("pause");
        }
    } while(choice != 0);
}

int main() {
    loadData();

    int choice;
    do {
        system("cls");
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
                system("pause");
                break;
            default:
                cout << "Pilihan salah!\n";
                system("pause");
        }
    } while(choice != 0);

    return 0;
}