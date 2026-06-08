#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <conio.h>
#include <windows.h>


static void cls() { system("cls"); }

static void setColor(int c) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); }

static void print(const std::string& s, int color = 7) { setColor(color); std::cout << s; setColor(7); }

static void printLn(const std::string& s, int color = 7) { print(s + "\n", color); }

// key generation 

static std::mt19937 rng(std::random_device{}());

static std::string randomSegment(int len, bool alphanumeric)
{
    const std::string chars = alphanumeric
        ? "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"
        : "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::uniform_int_distribution<int> dist(0, (int)chars.size() - 1);
    std::string out;
    out.reserve(len);
    for (int i = 0; i < len; i++) out += chars[dist(rng)];
    return out;
}

// Format is yk like super cool n stuff: HAKAI-XXXX-XXXX-XXXX
static std::string generateKey()
{
    return "HAKAI-" + randomSegment(4, true) + "-" +
        randomSegment(4, true) + "-" +
        randomSegment(4, true);
}


struct MenuItem { std::string label; };

static int runMenu(const std::string& title, const std::vector<MenuItem>& items)
{
    int sel = 0;
    while (true)
    {
        cls();
        printLn("  Hakai Key Generator", 11);
        printLn("  --------------------------------", 8);
        printLn("  " + title + "\n", 7);

        for (int i = 0; i < (int)items.size(); i++)
        {
            if (i == sel) { print("  > ", 14); printLn(items[i].label, 15); }
            else { printLn("    " + items[i].label, 8); }
        }

        printLn("\n  [Arrow keys] Navigate   [Enter] Select   [Esc] Back", 8);

        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            int arrow = _getch();
            if (arrow == 72) sel = (sel - 1 + (int)items.size()) % (int)items.size();
            if (arrow == 80) sel = (sel + 1) % (int)items.size();                    
        }
        else if (ch == 13) return sel;   
        else if (ch == 27) return -1;  
    }
}


static void showKeys(const std::vector<std::string>& keys)
{
    cls();
    printLn("  Hakai Key Generator", 11);
    printLn("  --------------------------------\n", 8);
    printLn("  Generated Keys:\n", 7);
    for (auto& k : keys) printLn("    " + k, 10);
    printLn("\n  Copy these and add them to KV with value  *", 8);
}

static std::vector<std::string> generateBatch(int count)
{
    std::vector<std::string> out;
    out.reserve(count);
    for (int i = 0; i < count; i++) out.push_back(generateKey());
    return out;
}

static void saveToFile(const std::vector<std::string>& keys)
{
    std::ofstream f("hakai_keys.txt", std::ios::app);
    for (auto& k : keys) f << k << "\n";
    f.close();
    printLn("\n  Saved to hakai_keys.txt", 14);
}


static void screenGenerate(int count)
{
    auto keys = generateBatch(count);
    showKeys(keys);
    printLn("\n  [S] Save to file   [Any other key] Back", 8);

    int ch = _getch();
    if (ch == 's' || ch == 'S') saveToFile(keys);
}

static void screenCustomCount()
{
    cls();
    printLn("  Hakai Key Generator", 11);
    printLn("  --------------------------------\n", 8);
    print("  How many keys? (1-500): ", 7);

    std::string input;
    std::getline(std::cin, input);

    int n = 0;
    try { n = std::stoi(input); }
    catch (...) { n = 0; }

    if (n < 1 || n > 500) { printLn("  Invalid number.", 12); Sleep(1200); return; }

    screenGenerate(n);
}

static void screenViewFile()
{
    cls();
    printLn("  Hakai Key Generator", 11);
    printLn("  --------------------------------\n", 8);

    std::ifstream f("hakai_keys.txt");
    if (!f.is_open()) { printLn("  No hakai_keys.txt found.", 12); _getch(); return; }

    printLn("  Saved keys:\n", 7);
    std::string line;
    int count = 0;
    while (std::getline(f, line)) { printLn("    " + line, 10); count++; }
    f.close();

    printLn("\n  Total: " + std::to_string(count) + " keys", 8);
    printLn("  [Any key] Back", 8);
    _getch();
}

static void screenClearFile()
{
    cls();
    printLn("  Hakai Key Generator", 11);
    printLn("  --------------------------------\n", 8);
    printLn("  Are you sure you want to clear hakai_keys.txt?", 12);
    printLn("  [Y] Yes   [Any other key] Cancel", 8);

    int ch = _getch();
    if (ch == 'y' || ch == 'Y')
    {
        std::ofstream f("hakai_keys.txt", std::ios::trunc);
        f.close();
        printLn("\n  File cleared.", 14);
        Sleep(1000);
    }
}


int main()
{
    SetConsoleTitleA("Hakai KEYGEN");


    CONSOLE_CURSOR_INFO ci; ci.dwSize = 1; ci.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);

    while (true)
    {
        int sel = runMenu("Main Menu", {
            { "Generate  1 key"   },
            { "Generate  5 keys"  },
            { "Generate 10 keys"  },
            { "Generate 25 keys"  },
            { "Generate 50 keys"  },
            { "Generate custom amount" },
            { "View saved keys"   },
            { "Clear saved keys"  },
            { "Exit"              },
            });

        if (sel == -1 || sel == 8) break;
        if (sel == 0) screenGenerate(1);
        if (sel == 1) screenGenerate(5);
        if (sel == 2) screenGenerate(10);
        if (sel == 3) screenGenerate(25);
        if (sel == 4) screenGenerate(50);
        if (sel == 5) screenCustomCount();
        if (sel == 6) screenViewFile();
        if (sel == 7) screenClearFile();
    }

    cls();
    printLn("  bye", 8);
    Sleep(600);
    return 0;
} // sorry for lack of comments, i dont really care for using them lol :3
