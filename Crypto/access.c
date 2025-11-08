#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// --- Integrity Part (Re-using HMAC from #10) ---
// Note: This is a placeholder, non-secure hash
unsigned int simple_hash(unsigned char *data, int len) {
    unsigned int hash = 5381;
    for (int i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + data[i];
    }
    return hash;
}

// Simplified HMAC function (64-byte block size)
void hmac(unsigned char *key, unsigned char *msg, unsigned int *mac_out) {
    unsigned char k_pad[64], k_ipad[64], k_opad[64];
    unsigned char ipad = 0x36;
    unsigned char opad = 0x5C;
    
    // 1. Prepare key
    memset(k_pad, 0, 64);
    memcpy(k_pad, key, strlen((char*)key));
    
    for (int i = 0; i < 64; i++) {
        k_ipad[i] = k_pad[i] ^ ipad;
        k_opad[i] = k_pad[i] ^ opad;
    }

    // 2. Inner Hash (k_ipad || message)
    unsigned char inner_data[1024]; // Simple buffer
    memcpy(inner_data, k_ipad, 64);
    memcpy(inner_data + 64, msg, strlen((char*)msg));
    unsigned int inner_hash = simple_hash(inner_data, 64 + strlen((char*)msg));

    // 3. Outer Hash (k_opad || inner_hash)
    unsigned char outer_data[1024];
    memcpy(outer_data, k_opad, 64);
    memcpy(outer_data + 64, &inner_hash, sizeof(unsigned int));
    *mac_out = simple_hash(outer_data, 64 + sizeof(unsigned int));
}
// --- End of Integrity Part ---


// --- User Database ---
// (Hardcoded for this simple simulation)
typedef struct {
    char username[50];
    char password[50];
    char role[10]; // "admin" or "user"
} User;

User user_db[] = {
    {"admin", "pass123", "admin"},
    {"bob", "pass456", "user"}
};
int num_users = 2;
// ---------------------


/**
 * @brief 1. AUTHENTICATION
 * Checks username and password.
 * @return The user's role ("admin", "user", or "none")
 */
char* authenticate_user() {
    char user[50], pass[50];
    printf("Enter username: ");
    scanf("%s", user);
    printf("Enter password: ");
    scanf("%s", pass);

    for (int i = 0; i < num_users; i++) {
        if (strcmp(user_db[i].username, user) == 0 && 
            strcmp(user_db[i].password, pass) == 0) {
            printf("\nAuthentication Successful. Welcome, %s!\n", user);
            return user_db[i].role;
        }
    }
    
    printf("\nAuthentication Failed. Invalid username or password.\n");
    return "none";
}

/**
 * @brief 3. INTEGRITY CHECK
 * Simulates reading a file and checking its MAC.
 */
void check_file_integrity() {
    printf("\n--- Checking File Integrity ---\n");
    
    // This is the "secret file"
    unsigned char file_data[] = "This is the super secret file content.";
    // This is the key it was signed with
    unsigned char integrity_key[] = "mac_key_123";
    
    // This is the "stored" MAC that we trust
    unsigned int stored_mac = 250367339; // Pre-calculated for this data/key
    
    // 1. Re-calculate the MAC on the current data
    unsigned int current_mac;
    hmac(integrity_key, file_data, &current_mac);
    
    printf("File Content: '%s'\n", file_data);
    printf("Stored MAC:   %u\n", stored_mac);
    printf("Current MAC:  %u\n", current_mac);
    
    // 2. Compare
    if (stored_mac == current_mac) {
        printf("Result: INTEGRITY OK. File has not been tampered with.\n");
    } else {
        printf("Result: FAILED! File data is corrupt or has been tampered with!\n");
    }
}

// --- Functions protected by Access Control ---
void admin_panel() {
    printf("\n>>> Welcome to the ADMIN PANEL (Access Granted).\n");
    printf("    You can delete the entire database.\n");
}

void user_dashboard() {
    printf("\n>>> Welcome to the USER DASHBOARD (Access Granted).\n");
    printf("    You can view your profile.\n");
    // All users can check file integrity
    check_file_integrity();
}
// ---------------------------------------------


int main() {
    // 1. AUTHENTICATION
    char* role = authenticate_user();
    
    // 2. ACCESS CONTROL
    printf("--- Access Control Check ---\n");
    printf("Your role is: %s\n", role);

    if (strcmp(role, "admin") == 0) {
        admin_panel();
        user_dashboard(); // Admins can also access user functions
    } else if (strcmp(role, "user") == 0) {
        // user_dashboard();
        // Trying to access admin panel will fail
        printf("\n(Attempting to access admin panel as 'user'...)\n");
        printf("    Access Denied to Admin Panel.\n");
        user_dashboard();
    } else {
        printf("Access Denied to all resources. Please log in.\n");
    }

    return 0;
}