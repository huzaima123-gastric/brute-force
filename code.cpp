// ======================= HEADER FILES ======================= 
// Input / output stream (cout, cerr) 
#include <iostream> 
// File handling (ifstream) 
#include <fstream> 
// String handling 
#include <string> 
// C-style string comparison (strcmp) 
#include <cstring> 
// crypt() function for hashing passwords (Linux) 
#include <unistd.h> 
// Time measurement (for performance calculation) 
#include <chrono> 
// Formatting output (not heavily used here) 
#include <iomanip> 
using namespace std; 
// ============================================================ 
// FUNCTION: extract_salt_hash 
// PURPOSE: 
//   - Reads one line from /etc/shadow 
//   - Checks if it belongs to the given username 
//   - Extracts hashing algorithm ID, salt, and stored hash 
// ============================================================ 
bool extract_salt_hash(const string& line, const string& username, 
                       string& salt_prefix, string& stored_hash) { 
    // Find first ':' → separates username and password field 
    size_t c1 = line.find(':'); 
    // If ':' not found OR username does not match → skip line 
    if (c1 == string::npos || line.substr(0, c1) != username) 
        return false; 
 
    // Find second ':' → end of password field 
    size_t c2 = line.find(':', c1 + 1); 
    if (c2 == string::npos) 
        return false; 
    // Extract password field between first and second colon 
    string field = line.substr(c1 + 1, c2 - c1 - 1); 
    // Validate that the field looks like a hashed password 
    // Format should start with '$' 
    if (field.size() < 10 || field[0] != '$') 
        return false; 
    // Find positions of '$' separators 
    size_t p1 = field.find('$', 1);         // after algorithm ID 
    size_t p2 = field.find('$', p1 + 1);    // after salt 
    // If structure is invalid 
    if (p1 == string::npos || p2 == string::npos) 
        return false; 
    // Extract hashing algorithm ID (e.g., "6" for SHA-512) 
    string id = field.substr(1, p1 - 1); 
    // Extract salt string 
    string salt_str = field.substr(p1 + 1, p2 - p1 - 1); 
    // Extract stored hash (everything after third '$') 
    stored_hash = field.substr(p2 + 1); 
    // Build salt prefix used by crypt() 
    // FORMAT: $id$salt$ 
    salt_prefix = "$" + id + "$" + salt_str + "$"; 
    return true; 
} 
// ============================================================ 
// FUNCTION: crack 
// PURPOSE: 
//   - Recursively generates passwords (a–z) 
//   - Hashes each candidate using crypt() 
//   - Compares with stored hash 
// ============================================================ 
bool crack(string current, int len, 
           const string& salt_prefix, const string& stored_hash) { 
    // Base case: generated password reached required length 
    if (current.length() == static_cast<size_t>(len)) { 
        // Hash the candidate password 
        const char* computed = crypt(current.c_str(), salt_prefix.c_str()); 
        if (computed) { 
            // Full hash = salt prefix + stored hash 
            string expected_full = salt_prefix + stored_hash; 
            // ================= DEBUG OUTPUT ================= 
            // Prints comparison details for known test password 
            if (current == "abcd") { 
                cout << "\n=== DEBUG - correct candidate 'abcd' reached ===\n"; 
                cout << "Salt prefix: " << salt_prefix << "\n"; 
                cout << "Stored hash: " << stored_hash << "\n"; 
                cout << "Computed full: " << computed << "\n"; 
                cout << "Expected full: " << expected_full << "\n"; 
                cout << "Match? " 
                     << (strcmp(computed, expected_full.c_str()) == 0 ? "YES" : "NO") 
                     << "\n"; 
                cout << "====================================================\n\n"; 
            } 
            // ================================================= 
            // Compare computed hash with actual stored hash 
            if (strcmp(computed, expected_full.c_str()) == 0) { 
                cout << "\nPASSWORD FOUND: " << current << endl; 
                cout << "Length: " << current.length() << " chars\n"; 
                return true; 
            } 
        } 
        return false; 
    } 
    // Recursive case: try all characters from a to z 
    for (char c = 'a'; c <= 'z'; ++c) { 
        if (crack(current + c, len, salt_prefix, stored_hash)) 
            return true; 
    } 
    return false; 
} 
// ============================================================ 
// MAIN FUNCTION 
// ============================================================ 
int main(int argc, char* argv[]) { 
    // Check if username argument is provided 
    if (argc != 2) { 
        cerr << "Usage: sudo " << argv[0] << " <username>\n"; 
        return 1; 
    } 
    // Store username 
    string user = argv[1]; 
    // Display program header 
    cout << "\n=== Password Cracker - DEBUG VERSION ===\n"; 
    cout << " User: " << user << "\n"; 
    cout << " Max length: 4 characters (a-z)\n\n"; 
    // Open /etc/shadow file (requires sudo) 
    ifstream shadow("/etc/shadow"); 
    if (!shadow.is_open()) { 
        cerr << "Cannot open /etc/shadow → must use sudo\n"; 
        return 1; 
    } 
    string line, salt, hash; 
    bool found = false; 
    // Read shadow file line by line 
    while (getline(shadow, line)) { 
        if (extract_salt_hash(line, user, salt, hash)) { 
            found = true; 
            break; 
        } 
    } 
    shadow.close(); 
    // If user not found or no password hash 
    if (!found) { 
        cerr << "User not found or has no password hash\n"; 
        return 1; 
    } 
    cout << "Starting brute-force...\n\n"; 
    // Record overall start time 
    auto t0 = chrono::steady_clock::now(); 
    bool cracked = false; 
    // Try password lengths from 1 to 7 
    for (int l = 1; l <= 7; ++l) { 
        cout << "Trying length " << l << "... "; 
        cout.flush(); 
        auto s = chrono::steady_clock::now(); 
        if (crack("", l, salt, hash)) { 
            cracked = true; 
            break; 
        } 
        auto e = chrono::steady_clock::now(); 
auto ms = chrono::duration_cast<chrono::milliseconds>(e - s).count(); 
cout << "finished (" << ms << " ms)\n"; 
} 
// Calculate total runtime 
auto total = chrono::duration_cast<chrono::milliseconds>( 
chrono::steady_clock::now() - t0).count(); 
// Final result output 
cout << "\n----------------------------------------\n"; 
cout << (cracked ? "SUCCESS!" : "Not found within 4 chars") << "\n"; 
cout << "Total time: " << total << " ms (" << total / 1000.0 << " sec)\n"; 
cout << "----------------------------------------\n"; 
return 0; 
}
