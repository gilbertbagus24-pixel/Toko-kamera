#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cstdlib>

using namespace std;

// ==========================================
// PENGATURAN WARNA ANSI (UNTUK GUI TERMINAL)
// ==========================================
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string WHITE   = "\033[37m";

// ==========================================
// STRUKTUR DATA
// ==========================================
struct Camera {
    int id;
    string brand;
    string model;
    double price;
    int stock;
};

const string FILENAME = "data_kamera.txt";

// ==========================================
// DEKLARASI FUNGSI (PROTOTYPES)
// ==========================================
// Fungsi UI / Terminal
void clearScreen();
void pauseScreen();
string center(string text, int width);
void drawTableBorderTop();
void drawTableBorderBottom();
void drawTableRow(const Camera& cam);
int getIntInput(string prompt);
double getDoubleInput(string prompt);

// Fungsi Utama CRUD
void addCamera(vector<Camera>& inventory);
void saveToFile(const vector<Camera>& inventory);
void loadFromFile(vector<Camera>& inventory);
void displayCameras(const vector<Camera>& inventory);

// Algoritma Lanjut (Advanced & Rekursi)
void recursivePrint(const vector<Camera>& inventory, size_t index);
int recursiveBinarySearch(const vector<Camera>& inventory, int low, int high, int searchId);
int partition(vector<Camera>& arr, int low, int high);
void quickSortByPrice(vector<Camera>& arr, int low, int high); // Rekursi QuickSort

// Helper & Pencarian
void sortCamerasById(vector<Camera>& inventory);
void searchCamera(vector<Camera> inventory); // copy by value

// ==========================================
// FUNGSI MAIN
// ==========================================
int main() {
    vector<Camera> inventory;
    loadFromFile(inventory);
    string choice;

    while (true) {
        clearScreen();
        // HEADER TAMPILAN GUI
        cout << CYAN << BOLD;
        cout << "  ╔═════════════════════════════════════════════════════════╗\n";
        cout << "  ║" << center("📸 PIXEL PERFECT CAMERA STORE", 57) << "║\n";
        cout << "  ╠═════════════════════════════════════════════════════════╣\n";
        cout << "  ║" << center("Admin Dashboard System", 57) << "║\n";
        cout << "  ╚═════════════════════════════════════════════════════════╝\n" << RESET;
        
        cout << "\n";
        cout << BOLD << "  [ MENU UTAMA ]\n" << RESET;
        cout << GREEN  << "  [1]" << RESET << " Tambah Data Kamera Baru\n";
        cout << GREEN  << "  [2]" << RESET << " Tampilkan Semua Daftar Kamera\n";
        cout << YELLOW << "  [3]" << RESET << " Urutkan Berdasarkan Harga " << MAGENTA << "(Quick Sort Rekursif)" << RESET << "\n";
        cout << YELLOW << "  [4]" << RESET << " Tampilkan Data " << MAGENTA << "(Print Rekursif)" << RESET << "\n";
        cout << BLUE   << "  [5]" << RESET << " Cari ID Kamera " << MAGENTA << "(Binary Search Rekursif)" << RESET << "\n";
        cout << RED    << "  [0]" << RESET << " Simpan & Keluar Aplikasi\n";
        
        cout << "\n  " << BOLD << "Masukkan pilihan menu > " << RESET;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == "1") {
            addCamera(inventory);
        } else if (choice == "2") {
            displayCameras(inventory);
            pauseScreen();
        } else if (choice == "3") {
            clearScreen();
            cout << "\n" << BOLD << CYAN << "  [>] Memproses Pengurutan Data (Quick Sort)..." << RESET << "\n";
            if (!inventory.empty()) {
                quickSortByPrice(inventory, 0, inventory.size() - 1);
                saveToFile(inventory);
                cout << GREEN << "  [✓] Data berhasil diurutkan dari Termurah ke Termahal!" << RESET << "\n";
            } else {
                cout << RED << "  [!] Data masih kosong." << RESET << "\n";
            }
            pauseScreen();
        } else if (choice == "4") {
            clearScreen();
            cout << BOLD << CYAN << "  === MENAMPILKAN DATA (PRINT REKURSIF) ===" << RESET << "\n\n";
            if (inventory.empty()) {
                cout << RED << "  [!] Belum ada data kamera." << RESET << "\n";
            } else {
                drawTableBorderTop();
                recursivePrint(inventory, 0);
                drawTableBorderBottom();
            }
            pauseScreen();
        } else if (choice == "5") {
            searchCamera(inventory);
        } else if (choice == "0") {
            clearScreen();
            saveToFile(inventory);
            cout << GREEN << BOLD << "\n  [✓] Data otomatis tersimpan (Fstream)." << RESET << "\n";
            cout << CYAN << BOLD << "  [!] Terima kasih telah menggunakan Pixel Perfect System!\n\n" << RESET;
            break;
        } else {
            cout << RED << "\n  [!] Pilihan tidak valid! Silakan masukkan nomor menu yang benar." << RESET << "\n";
            pauseScreen(); 
        }
    }
    return 0;
}

// ==========================================
// IMPLEMENTASI FUNGSI UI & VALIDASI
// ==========================================
void clearScreen() {
    // Clear screen menggunakan escape code agar bebas dari flicker system()
    cout << "\033[2J\033[1;1H"; 
}

void pauseScreen() {
    cout << "\n" << YELLOW << "  ► Tekan Enter untuk kembali ke menu..." << RESET;
    cin.get();
}

string center(string text, int width) {
    int len = text.length();
    // Mengakali karakter emoji Unicode length 
    // (emoji kamera mengambil byte ekstra, tapi kita asumsikan rata tengah sederhana)
    int offset = 0;
    if(text.find("📸") != string::npos) offset = 2; 

    if (width < (len - offset)) return text;
    int diff = width - (len - offset);
    int pad1 = diff / 2;
    int pad2 = diff - pad1;
    return string(pad1, ' ') + text + string(pad2, ' ');
}

int getIntInput(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << RED << "      [!] Input tidak valid! Harus berupa angka integer." << RESET << "\n";
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
            cout << RED << "      [!] Input tidak valid! Harus berupa angka." << RESET << "\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// ==========================================
// IMPLEMENTASI TABEL (GUI)
// ==========================================
void drawTableBorderTop() {
    cout << CYAN;
    cout << "  ╔══════╦═════════════════╦══════════════════════╦═════════════════╦════════╗\n";
    cout << "  ║ " << WHITE << BOLD << left << setw(4) << "ID" << CYAN << " ║ " 
         << WHITE << setw(15) << "Merek" << CYAN << " ║ " 
         << WHITE << setw(20) << "Model" << CYAN << " ║ " 
         << WHITE << setw(15) << "Harga (Rp)" << CYAN << " ║ " 
         << WHITE << setw(6) << "Stok" << CYAN << " ║\n";
    cout << "  ╠══════╬═════════════════╬══════════════════════╬═════════════════╬════════╣\n";
    cout << RESET;
}

void drawTableBorderBottom() {
    cout << CYAN;
    cout << "  ╚══════╩═════════════════╩══════════════════════╩═════════════════╩════════╝\n";
    cout << RESET;
}

void drawTableRow(const Camera& cam) {
    cout << CYAN << "  ║ " << RESET << left << setw(4) << cam.id 
         << CYAN << " ║ " << RESET << left << setw(15) << cam.brand.substr(0,15)
         << CYAN << " ║ " << RESET << left << setw(20) << cam.model.substr(0,20)
         << CYAN << " ║ " << RESET << left << setw(15) << fixed << setprecision(0) << cam.price
         << CYAN << " ║ " << RESET << left << setw(6) << cam.stock 
         << CYAN << " ║\n" << RESET;
}

// ==========================================
// IMPLEMENTASI FUNGSI CRUD & FSTREAM
// ==========================================
void addCamera(vector<Camera>& inventory) {
    clearScreen();
    cout << GREEN << BOLD << "  === TAMBAH DATA KAMERA ===" << RESET << "\n\n";
    Camera newCam;
    
    newCam.id = getIntInput("  [>] ID Kamera    : ");
    
    cout << "  [>] Merek Kamera : ";
    getline(cin, newCam.brand);
    
    cout << "  [>] Model Kamera : ";
    getline(cin, newCam.model);
    
    newCam.price = getDoubleInput("  [>] Harga (Rp)   : ");
    newCam.stock = getIntInput("  [>] Stok         : ");

    inventory.push_back(newCam);
    saveToFile(inventory); // Simpan langsung ke file
    
    cout << "\n" << GREEN << "  [✓] Data berhasil ditambahkan dan disimpan di database!" << RESET << "\n";
    pauseScreen();
}

void saveToFile(const vector<Camera>& inventory) {
    ofstream file(FILENAME);
    if (file.is_open()) {
        for (const auto& cam : inventory) {
            file << cam.id << "," << cam.brand << "," << cam.model << "," << fixed << setprecision(0) << cam.price << "," << cam.stock << "\n";
        }
        file.close();
    }
}

void loadFromFile(vector<Camera>& inventory) {
    inventory.clear();
    ifstream file(FILENAME);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            Camera cam;
            size_t p1 = line.find(',');
            size_t p2 = line.find(',', p1 + 1);
            size_t p3 = line.find(',', p2 + 1);
            size_t p4 = line.find(',', p3 + 1);

            if (p1 != string::npos && p2 != string::npos && p3 != string::npos && p4 != string::npos) {
                cam.id = stoi(line.substr(0, p1));
                cam.brand = line.substr(p1 + 1, p2 - p1 - 1);
                cam.model = line.substr(p2 + 1, p3 - p2 - 1);
                cam.price = stod(line.substr(p3 + 1, p4 - p3 - 1));
                cam.stock = stoi(line.substr(p4 + 1));
                inventory.push_back(cam);
            }
        }
        file.close();
    }
}

void displayCameras(const vector<Camera>& inventory) {
    clearScreen();
    cout << BOLD << CYAN << "  === DAFTAR SEMUA KAMERA (FSTREAM) ===" << RESET << "\n\n";
    if (inventory.empty()) {
        cout << RED << "  [!] Belum ada data kamera." << RESET << "\n";
        return;
    }
    
    drawTableBorderTop();
    for (const auto& cam : inventory) {
        drawTableRow(cam);
    }
    drawTableBorderBottom();
}

// ==========================================
// ALGORITMA REKURSIF (PRINT & BINARY SEARCH)
// ==========================================
void recursivePrint(const vector<Camera>& inventory, size_t index) {
    // Base Case
    if (index >= inventory.size()) return; 
    
    // Cetak row saat ini
    drawTableRow(inventory[index]);
    
    // Recursive Step
    recursivePrint(inventory, index + 1);
}

int recursiveBinarySearch(const vector<Camera>& inventory, int low, int high, int searchId) {
    // Base case 1: Data tidak ditemukan
    if (low > high) return -1;
    
    int mid = low + (high - low) / 2;
    
    // Base case 2: Data ditemukan
    if (inventory[mid].id == searchId) return mid;
    
    // Recursive step
    if (inventory[mid].id > searchId) 
        return recursiveBinarySearch(inventory, low, mid - 1, searchId);
        
    return recursiveBinarySearch(inventory, mid + 1, high, searchId);
}

// ==========================================
// ALGORITMA SORTING LANJUTAN (QUICK SORT)
// ==========================================
int partition(vector<Camera>& arr, int low, int high) {
    double pivot = arr[high].price; // Pivot berdasarkan harga
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].price < pivot) { // Pengurutan Ascending
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSortByPrice(vector<Camera>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        // Pemanggilan rekursif QuickSort
        quickSortByPrice(arr, low, pi - 1);
        quickSortByPrice(arr, pi + 1, high);
    }
}

// Sorting dasar (Bubble Sort) khusus untuk Binary Search ID (karena harus urut by ID)
void sortCamerasById(vector<Camera>& inventory) {
    int n = inventory.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (inventory[j].id > inventory[j + 1].id) {
                swap(inventory[j], inventory[j + 1]);
            }
        }
    }
}

// Wrapper fungsi pencarian
void searchCamera(vector<Camera> inventory) {
    clearScreen();
    cout << BLUE << BOLD << "  === PENCARIAN KAMERA (BINARY SEARCH) ===" << RESET << "\n\n";
    
    if (inventory.empty()) {
        cout << RED << "  [!] Data kosong." << RESET << "\n";
        pauseScreen();
        return;
    }

    // Persyaratan Binary Search: Array harus terurut berdasarkan ID
    sortCamerasById(inventory); 

    int searchId = getIntInput("  [?] Masukkan ID Kamera yang dicari: ");

    // Panggil fungsi rekursif
    int index = recursiveBinarySearch(inventory, 0, inventory.size() - 1, searchId);

    if (index != -1) {
        cout << "\n" << GREEN << "  [✓] Kamera Ditemukan!" << RESET << "\n\n";
        drawTableBorderTop();
        drawTableRow(inventory[index]);
        drawTableBorderBottom();
    } else {
        cout << "\n" << RED << "  [X] Kamera dengan ID " << searchId << " tidak ditemukan." << RESET << "\n";
    }
    pauseScreen();
}
