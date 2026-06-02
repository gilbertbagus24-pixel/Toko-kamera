#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <string>
using namespace std;

struct Product {
  int id;
  string name;
  double price;
  int stock;
};

struct Transaction {
  int id;
  int productId;
  int qty;
  double totalPrice;
  bool isReturned;
};

struct Account{
  string username;
  string passwordHash;
  string role;
};

vector<Product> products;
vector<Transaction> transactions;
vector<Account> accounts;

int getIntInput(string prompt) {
  int value;
  while (true) {
    cout << prompt;
    if (cin >> value) {
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      return value;
    } else {
      cout << "Input tidak valid! Harus berupa angka.\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  }
}

double getDoubleInput(string prompt) {
  double value;
  while (true) {
    cout << prompt;
    if (cin >> value) {
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      return value;
    } else {
      cout << "Input tidak valid! Harus berupa angka.\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  }
}

void saveData() {
  ofstream file("data.txt");
  for (auto p : products) {
    file << p.id << "," << p.name << "," << p.price << "," << p.stock << endl;
  }
  file.close();
  cout << "Data berhasil disimpan!\n";
}

void loadData() {
  ifstream file("data.txt");
  if (!file.is_open())
    return;

  products.clear();
  Product p;
  char comma;
  while (file >> p.id >> comma) {
    getline(file, p.name, ',');
    file >> p.price >> comma >> p.stock;
    products.push_back(p);
  }
  file.close();
}

void saveTransactions() {
  ofstream file("transactions.txt");
  for (auto t : transactions) {
    file << t.id << "," << t.productId << "," << t.qty << "," << t.totalPrice
         << "," << t.isReturned << endl;
  }
  file.close();
}

void loadTransactions() {
  ifstream file("transactions.txt");
  if (!file.is_open())
    return;

  transactions.clear();
  Transaction t;
  char comma;
  while (file >> t.id >> comma >> t.productId >> comma >> t.qty >> comma >>
         t.totalPrice >> comma >> t.isReturned) {
    transactions.push_back(t);
  }
  file.close();
}

void addProduct() {
  Product p;

  p.id = getIntInput("\nID Produk   : ");

  for (auto existing : products) {
    if (existing.id == p.id) {
      cout << "ID sudah terdaftar!\n";
      return;
    }
  }

  cout << "Nama Produk : ";
  getline(cin, p.name);

  p.price = getDoubleInput("Harga       : ");
  p.stock = getIntInput("Stock       : ");

  products.push_back(p);
  cout << "Produk berhasil ditambahkan!\n";
}

void insertionSort(bool ascending = true) {
  int n = (int)products.size();
  for (int i = 1; i < n; i++) {
    Product key = products[(size_t)i];
    int j = i - 1;

    if (ascending) {
      while (j >= 0 && products[(size_t)j].price > key.price) {
        products[(size_t)(j + 1)] = products[(size_t)j];
        j = j - 1;
      }
    } else {
      while (j >= 0 && products[(size_t)j].price < key.price) {
        products[(size_t)(j + 1)] = products[(size_t)j];
        j = j - 1;
      }
    }
    products[(size_t)(j + 1)] = key;
  }
  if (ascending) {
    cout << "Produk berhasil diurutkan berdasarkan harga termurah!\n";
  } else {
    cout << "Produk berhasil diurutkan berdasarkan harga termahal!\n";
  }
}

void showProducts(bool isBuyer = false) {
  char stayChoice;
  do {
    cout << "\n===== LIST PRODUK =====\n";

    if (products.empty()) {
      cout << "Belum ada produk.\n";
      return;
    }

    cout << fixed << setprecision(0);

    for (auto p : products) {
      cout << p.id << " | " << p.name << " | Rp" << p.price
           << " | Stock: " << p.stock << endl;
    }

    char sortChoice;
    cout << "\nIngin mengurutkan produk? (y/n): ";
    cin >> sortChoice;

    if (sortChoice == 'y' || sortChoice == 'Y') {
      int orderChoice;
      cout << "Pilih urutan harga:\n";
      cout << "1. Termurah ke Termahal (Ascending)\n";
      cout << "2. Termahal ke Termurah (Descending)\n";
      orderChoice = getIntInput("Pilih (1-2): ");

      if (orderChoice == 2) {
        insertionSort(false);
      } else {
        insertionSort(true);
      }

      cout << "\n===== LIST PRODUK (TERURUT) =====\n";
      for (auto p : products) {
        cout << p.id << " | " << p.name << " | Rp" << p.price
             << " | Stock: " << p.stock << endl;
      }
    }

    if (isBuyer) {
      cout << "\nApakah ingin tetap di halaman list produk? (y/n): ";
      cin >> stayChoice;
    } else {
      stayChoice = 'n';
    }
  } while (stayChoice == 'y' || stayChoice == 'Y');
}

void printReceipt(Transaction t, Product p){
  string filename = "strux_" + to_string(t.id) + ".txt";

  ofstream file(filename);


  if (!file.is_open()){
    cout << "Gagal membuat struk!\n";
    return;
  }

  file << fixed << setprecision(0);

  time_t now = time(0);
  tm *ltm = localtime(&now);

  file << "+--------------------------------+\n";
  file << "|         KREATIV STORE          |\n";
  file << "+--------------------------------+\n";
  file << "| ID TX    : " << left << setw(20) << t.id << "|\n";
  string tanggal = 
      to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);

  file << "| Tanggal  : " << left << setw(20) << tanggal << "|\n";
  string jam = to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);
  file << "| Jam      : " << left << setw(20) << jam << "|\n";
  file << "| Produk   : " << left << setw(20) << p.name << "|\n";
  file << "| Qty      : " << left << setw(20) << t.qty << "|\n";
  file << "| Harga    : Rp" << left << setw(18) << p.price << "|\n";

  file << "+--------------------------------+\n";

  file << "| TOTAL    : Rp" << left << setw(18) << t.totalPrice << "|\n";

  file << "+--------------------------------+\n";

  file << "| Terimakasih telah menyewa!     |\n";
  file << "+--------------------------------+\n";

  file.close();

  cout << "Struk berhasil dicetak: " << filename << endl;
  
  
}

void rentProduct() {
  int id, qty;
  bool found = false;

  cout << "\n=== TRANSAKSI SEWA PRODUK ===\n";
  id = getIntInput("Masukkan ID produk yang ingin disewa: ");

  cout << fixed << setprecision(0);

  for (auto &p : products) {
    if (p.id == id) {
      found = true;

      cout << "\n--- DETAIL PRODUK (DATABASE) ---\n";
      cout << "ID           : " << p.id << endl;
      cout << "Nama Produk  : " << p.name << endl;
      cout << "Harga Sewa   : Rp" << p.price << " / unit" << endl;
      cout << "Stok Tersedia: " << p.stock << endl;
      cout << "--------------------------------\n";

      if (p.stock <= 0) {
        cout << "Maaf, stok produk saat ini sedang habis!\n";
        return;
      }

      qty = getIntInput("\nMasukkan jumlah sewa: ");

      if (qty <= 0) {
        cout << "Jumlah sewa tidak valid!\n";
        return;
      }

      if (qty > p.stock) {
        cout << "Stok tidak mencukupi!\n";
        return;
      }

      p.stock -= qty;

      double total = qty * p.price;

      int newTxId = 1001;
      if (!transactions.empty()) {
        newTxId = transactions.back().id + 1;
      }

      Transaction t;
      t.id = newTxId;
      t.productId = p.id;
      t.qty = qty;
      t.totalPrice = total;
      t.isReturned = false;

      transactions.push_back(t);

      printReceipt(t, p);

      cout << "\n[TRANSAKSI BERHASIL]\n";
      cout << "ID Transaksi: " << t.id << endl;
      cout << "Produk      : " << p.name << endl;
      cout << "Jumlah Sewa : " << qty << " unit" << endl;
      cout << "Total Bayar : Rp" << total << endl;
      cout << "---------------------\n";

      saveData();         // Otomatis simpan perubahan stok ke database file
      saveTransactions(); // Otomatis simpan transaksi baru
      return;
    }
  }

  if (!found)
    cout << "Produk tidak ditemukan di database!\n";
}

void showTransactions() {
  cout << "\n===== RIWAYAT TRANSAKSI =====\n";
  if (transactions.empty()) {
    cout << "Belum ada transaksi.\n";
    return;
  }

  cout << "ID Tx | ID Prod | Nama Produk            | Qty | Total Bayar  | "
          "Status\n";
  cout << "--------------------------------------------------------------------"
          "-----\n";
  for (auto t : transactions) {
    string prodName = "Tidak ditemukan";
    for (auto p : products) {
      if (p.id == t.productId) {
        prodName = p.name;
        break;
      }
    }

    cout << t.id << "  | " << t.productId << "       | ";

    if (prodName.length() > 22u) {
      cout << prodName.substr(0, 19) << "...";
    } else {
      cout << prodName;
      for (size_t i = prodName.length(); i < 22u; ++i)
        cout << " ";
    }

    cout << " | " << t.qty << "   | Rp" << t.totalPrice << " | "
         << (t.isReturned ? "Returned" : "Rented") << endl;
  }
}

void returnProduct() {
  cout << "\n=== PENGEMBALIAN PRODUK ===\n";
  if (transactions.empty()) {
    cout << "Belum ada transaksi di database.\n";
    return;
  }

  int txId = getIntInput("Masukkan ID Transaksi: ");
  bool found = false;

  for (auto &t : transactions) {
    if (t.id == txId) {
      found = true;
      if (t.isReturned) {
        cout << "Transaksi ini sudah dikembalikan sebelumnya!\n";
        return;
      }

      bool productFound = false;
      string prodName = "";
      for (auto &p : products) {
        if (p.id == t.productId) {
          p.stock += t.qty;
          prodName = p.name;
          productFound = true;
          break;
        }
      }

      t.isReturned = true;
      saveTransactions();
      saveData();

      cout << "\n[PENGEMBALIAN BERHASIL]\n";
      cout << "ID Transaksi: " << t.id << endl;
      if (productFound) {
        cout << "Produk      : " << prodName << endl;
        cout << "Jumlah      : " << t.qty
             << " unit telah dikembalikan ke stok.\n";
      } else {
        cout << "Produk ID   : " << t.productId
             << " (Produk tidak ditemukan di database saat ini, tetapi status "
                "transaksi telah diperbarui).\n";
      }
      cout << "---------------------\n";
      return;
    }
  }

  if (!found) {
    cout << "ID Transaksi tidak ditemukan!\n";
  }
}

string simpleHash(string password){

  int hash = 0;

  for (char c : password){
      hash += c * 7;
  }

  return to_string(hash);
}

void saveAccounts(){

  ofstream file("accounts.txt");

  for (auto a : accounts){

    file << a.username << ",";
    file << a.passwordHash << ",";
    file << a.role << endl;
  }
  file.close();
}

void loadAccounts(){

  ifstream file("accounts.txt");

  if (!file.is_open())
      return;

  accounts.clear();

  Account a;

  //char comma;

  while (getline(file, a.username, ',')){

    getline(file, a.passwordHash, ',');
    getline (file, a.role);
    accounts.push_back(a);
  }
    file.close();
}

void registerAccount(){

    Account a;

    cout << "\n=== REGISTER ACCOUNT ===\n";

    while (true) {

        cout << "Username: ";
        getline(cin, a.username);

        if (a.username.empty()) {
            cout << "Username tidak boleh kosong!\n";
            continue;
        }

        if (a.username.find(' ') != string::npos) {
            cout << "Username tidak boleh mengandung spasi!\n";
            continue;
        }

        bool used = false;

        for (auto acc : accounts) {

            if (acc.username == a.username) {
                used = true;
                break;
            }
        }

        if (used) {
            cout << "Username sudah digunakan!\n";
            continue;
        }

        break;
    }

    string password;

    cout << "Password: ";
    getline(cin, password);

    a.passwordHash = simpleHash(password);
    a.role = "buyer";

    accounts.push_back(a);

    saveAccounts();

    cout << "Register berhasil!\n";
}

bool login(string role) {
  system("cls");
  string username, password;
  cout << "\nUsername: ";
  cin >> username;
  cout << "Password: ";
  cin >> password;

  string hashed = simpleHash(password);

  for (auto a : accounts){

    if (a.username == username && a.passwordHash == hashed && a.role == role) {

      cout << "Login Berhasil!\n";
      system("pause");

      return true;
    }
  }
 // if (username == "admin" && password == "admin123") {
   // cout << "Login Berhasil!\n";
    //system("pause");
    //return true;
  //} else {
    cout << "Login Gagal!\n";
    system("pause");
    return false;
  //}
}

void editProduct() {
    if (products.empty()) {
        cout << "Belum ada produk.\n";
        return;
    }

    int id = getIntInput("Masukkan ID produk yang ingin diedit: ");

    for (auto &p : products) {

        if (p.id == id) {

            cout << "\n=== DATA LAMA ===\n";
            cout << "Nama  : " << p.name << endl;
            cout << "Harga : Rp" << p.price << endl;
            cout << "Stok  : " << p.stock << endl;

            cout << "\n=== DATA BARU ===\n";

            cout << "Nama Produk : ";
            getline(cin, p.name);

            p.price = getDoubleInput("Harga Baru : ");
            p.stock = getIntInput("Stok Baru  : ");

            saveData();

            cout << "\nProduk berhasil diperbarui!\n";
            return;
        }
    }

    cout << "Produk tidak ditemukan!\n";
}

void deleteProduct() {

    if (products.empty()) {
        cout << "Belum ada produk.\n";
        return;
    }

    int id = getIntInput("Masukkan ID produk yang ingin dihapus: ");

    for (size_t i = 0; i < products.size(); i++) {

        if (products[i].id == id) {

            cout << "\nProduk ditemukan:\n";
            cout << products[i].name << endl;

            char confirm;

            cout << "Yakin ingin menghapus? (y/n): ";
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {

                products.erase(products.begin() + i);

                saveData();

                cout << "Produk berhasil dihapus!\n";
            }
            else {
                cout << "Penghapusan dibatalkan.\n";
            }

            return;
        }
    }

    cout << "Produk tidak ditemukan!\n";
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
    cout << "5. Pengembalian Produk\n";
    cout << "6. Tampilkan Transaksi\n";
    cout << "7. Edit Produk\n";
    cout << "8. Hapus Produk\n";
    cout << "0. Log Out\n";
    choice = getIntInput("Pilih: ");

    switch (choice) {
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
    case 5:
      returnProduct();
      system("pause");
      break;
    case 6:
      showTransactions();
      system("pause");
      break;
    case 7:
      editProduct();
      system("pause");
      break;
    case 8:
      deleteProduct();
      system("pause");
      break;
    case 0:
      break;
    default:
      cout << "Pilihan salah!\n";
      system("pause");
    }
  } while (choice != 0);
}

void searchProduct(){
  if(products.empty()){
    cout << "\nBelum ada produk.\n";
    return;
  }

  string keyword;

  cout << "\n=== CARI PRODUK ===\n";
  cout << "Masukan nama produk: ";
  getline(cin, keyword);

  transform(keyword.begin(), keyword.end(),keyword.begin(), ::tolower);

  bool found = false;

  cout << "\n==== HASIL PENCARIAN ====\n";

  cout << fixed << setprecision(0);

  for (auto p : products){
    string productName = p.name;

    transform(productName.begin(), productName.end(), productName.begin(), ::tolower);

    if(productName.find(keyword) != string::npos){
      cout << p.id << " | " << p.name << " | Rp" << p.price << "| Stock: " << p.stock << endl;

      found =true;

    }

  }
  if (!found){
    cout << "Produk tidak ditemukan.\n";

  }
}


void buyerMenu() {
  int choice;
  do {
    system("cls");
    cout << "\n===== MENU PEMBELI =====\n";
    cout << "1. Tampilkan Produk\n";
    cout << "2. Sewa Produk\n";
    cout << "3. Cari Produk\n";
    cout << "0. Kembali\n";
    choice = getIntInput("Pilih: ");

    switch (choice) {
    case 1:
      showProducts(true);
      system("pause");
      break;
    case 2:
      rentProduct();
      system("pause");
      break;
    case 3:
      searchProduct();
      system("pause");
      break;
    case 0:
      break;
    default:
      cout << "Pilihan salah!\n";
      system("pause");
    }
  } while (choice != 0);
}

int main() {
  loadData();
  loadTransactions();
  loadAccounts();
  if(accounts.empty()){

    Account admin;

    admin.username = "admin";
    admin.passwordHash = simpleHash("admin123");

    admin.role = "admin";

    accounts.push_back(admin);

    saveAccounts();
  }

  int choice;
  do {
    system("cls");
    cout << "\n===== KREATIV STORE =====\n";
    cout << "Masuk Sebagai:\n";
    cout << "1. Admin\n";
    cout << "2. Pembeli\n";
    cout << "3. Register\n";
    cout << "0. Keluar\n";
    choice = getIntInput("Pilih: ");

    switch (choice) {
    case 1:
      if (login("admin")) {
        adminMenu();
      }
      break;
    case 2:
      if (login("buyer")) {
        buyerMenu();
      }
      break;
    case 3:
      registerAccount();
      system("pause");
      break;
    case 0:
      saveData();
      saveTransactions();
      cout << "Program selesai.\n";
      system("pause");
      break;
    default:
      cout << "Pilihan salah!\n";
      system("pause");
    }
  } while (choice != 0);

  return 0;
}
