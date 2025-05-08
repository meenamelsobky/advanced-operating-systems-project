#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to track requests and their processing order
typedef struct {
    int cylinder;    // Cylinder number
    int is_endpoint; // Flag to indicate if this is an endpoint (boundary) of the disk
    int is_original; // Flag to indicate if this was one of the original requests
} Request;

// Function prototypes
int fcfs(int requests[], int num, int initial_pos, Request result_order[], int *order_count);
int scan(int requests[], int num, int initial_pos, int total_cylinders, char direction, Request result_order[], int *order_count);
int c_scan(int requests[], int num, int initial_pos, int total_cylinders, char direction, Request result_order[], int *order_count);
void bubbleSort(int arr[], int n);
void swap(int* a, int* b);

int main() {
    int *requests;           // Original requests array
    Request *result_order;   // Array to store order of processing with additional info
    int num_requests;
    int initial_position;
    int total_cylinders;
    int head_movement;
    char algorithm[10];
    char input_choice;
    char direction = 'u';    // Default direction is up
    int order_count = 0;     // Count of items in the result_order array
    char continue_choice;    // For continuing or exiting the program

    // Get total number of cylinders
    printf("Enter the total number of cylinders: ");
    scanf("%d", &total_cylinders);
    
    if (total_cylinders <= 0) {
        printf("Number of cylinders must be positive.\n");
        return 1;
    }

    // Get queue size (number of requests)
    printf("Enter the queue size (number of requests): ");
    scanf("%d", &num_requests);
    
    if (num_requests <= 0) {
        printf("Queue size must be positive.\n");
        return 1;
    }

    // Allocate memory for requests array and result order array (adding extra space for endpoints)
    requests = (int*)malloc(num_requests * sizeof(int));
    result_order = (Request*)malloc((num_requests + 4) * sizeof(Request)); // Extra space for endpoints
    
    if (requests == NULL || result_order == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Ask if the user wants to input the queue or generate random requests
    printf("Do you want to enter the queue manually (m) or generate random requests (r)? ");
    scanf(" %c", &input_choice);

    if (input_choice == 'm' || input_choice == 'M') {
        // Get queue elements from user
        printf("Enter %d cylinder requests (0 to %d):\n", num_requests, total_cylinders - 1);
        for (int i = 0; i < num_requests; i++) {
            scanf("%d", &requests[i]);
            
            // Validate the request
            if (requests[i] < 0 || requests[i] >= total_cylinders) {
                printf("Invalid request. Must be between 0 and %d.\n", total_cylinders - 1);
                i--; // Repeat this iteration
            }
        }
    } else {
        // Generate random requests
        srand(time(NULL));
        printf("Generating %d random requests...\n", num_requests);
        for (int i = 0; i < num_requests; i++) {
            requests[i] = rand() % total_cylinders;
        }
        
        // Display the generated requests
        printf("Generated requests: ");
        for (int i = 0; i < num_requests; i++) {
            printf("%d ", requests[i]);
            if ((i + 1) % 10 == 0 && i != num_requests - 1) {
                printf("\n                  ");
            }
        }
        printf("\n");
    }

    // Get initial position of disk head
    printf("Enter the initial position of the disk head (0 to %d): ", total_cylinders - 1);
    scanf("%d", &initial_position);
    
    if (initial_position < 0 || initial_position >= total_cylinders) {
        printf("Initial position must be between 0 and %d.\n", total_cylinders - 1);
        free(requests);
        free(result_order);
        return 1;
    }

    // Loop to allow multiple algorithm runs
    do {
        // Reset order_count for each run
        order_count = 0;

        // Get scheduling algorithm
        printf("\nEnter the scheduling algorithm (FCFS, SCAN, C-SCAN): ");
        scanf("%s", algorithm);

        // Convert algorithm to uppercase
        for (int i = 0; algorithm[i]; i++) {
            if (algorithm[i] >= 'a' && algorithm[i] <= 'z') {
                algorithm[i] = algorithm[i] - 'a' + 'A';
            }
        }

        // If algorithm is SCAN or C-SCAN, ask for direction
        if (strcmp(algorithm, "SCAN") == 0 || strcmp(algorithm, "C-SCAN") == 0) {
            printf("Enter the initial direction (u for up, d for down): ");
            scanf(" %c", &direction);
            
            if (direction != 'u' && direction != 'U' && direction != 'd' && direction != 'D') {
                printf("Invalid direction. Using default (up).\n");
                direction = 'u';
            }
        }

        // Calculate total head movement based on selected algorithm
        if (strcmp(algorithm, "FCFS") == 0) {
            head_movement = fcfs(requests, num_requests, initial_position, result_order, &order_count);
            printf("\nFCFS Scheduling:\n");
        } else if (strcmp(algorithm, "SCAN") == 0) {
            head_movement = scan(requests, num_requests, initial_position, total_cylinders, direction, result_order, &order_count);
            printf("\nSCAN Scheduling (Direction: %c):\n", direction);
        } else if (strcmp(algorithm, "C-SCAN") == 0) {
            head_movement = c_scan(requests, num_requests, initial_position, total_cylinders, direction, result_order, &order_count);
            printf("\nC-SCAN Scheduling (Direction: %c):\n", direction);
        } else {
            printf("Invalid algorithm. Please enter FCFS, SCAN, or C-SCAN.\n");
            continue;
        }

        // Print the result: order of requests served and seek time
        printf("\nOrder of requests served:\n");
        printf("%-12s %-12s %-12s\n", "Order", "Cylinder", "Type");
        printf("-------------------------------------\n");
        printf("%-12s %-12d %-12s\n", "Initial", initial_position, "Initial");
        
        for (int i = 0; i < order_count; i++) {
            printf("%-12d %-12d %-12s\n", i+1, result_order[i].cylinder, 
                   result_order[i].is_endpoint ? "Boundary" : 
                   (result_order[i].is_original ? "Request" : "Jump"));
        }
        
        printf("\nTotal seek time: %d cylinders\n", head_movement);

        // Ask if the user wants to try another algorithm
        printf("\nDo you want to try another algorithm on the same requests? (y/n): ");
        scanf(" %c", &continue_choice);
    } while (continue_choice == 'y' || continue_choice == 'Y');

    // Free allocated memory
    free(requests);
    free(result_order);
    
    printf("Exiting program.\n");
    return 0;
}

// First Come First Served algorithm
int fcfs(int requests[], int num, int initial_pos, Request result_order[], int *order_count) {
    int head_movement = 0;
    int current_pos = initial_pos;
    
    *order_count = 0;
    
    for (int i = 0; i < num; i++) {
        // Calculate absolute distance (seek time)
        head_movement += abs(current_pos - requests[i]);
        
        // Record this request
        result_order[*order_count].cylinder = requests[i];
        result_order[*order_count].is_endpoint = 0;
        result_order[*order_count].is_original = 1;
        (*order_count)++;
        
        current_pos = requests[i];
    }
    
    return head_movement;
}

// SCAN algorithm (Elevator)
int scan(int requests[], int num, int initial_pos, int total_cylinders, char direction, Request result_order[], int *order_count) {
    int head_movement = 0;
    int current_pos = initial_pos;
    int *temp_requests;
    
    *order_count = 0;
    
    // Make a copy of the requests, adding both boundaries
    temp_requests = (int*)malloc((num + 2) * sizeof(int)); // +2 for boundaries
    if (temp_requests == NULL) {
        printf("Memory allocation failed.\n");
        return -1;
    }
    
    // Add boundary requests (0 and total_cylinders - 1)
    temp_requests[0] = 0;
    temp_requests[1] = total_cylinders - 1;
    for (int i = 0; i < num; i++) {
        temp_requests[i + 2] = requests[i];
    }
    num += 2; // Update number of requests to include boundaries
    
    // Sort all requests, including boundaries
    bubbleSort(temp_requests, num);
    
    // Find position where requests are >= initial_pos
    int idx = 0;
    while (idx < num && temp_requests[idx] < initial_pos) {
        idx++;
    }
    
    if (direction == 'u' || direction == 'U') {
        // First serve all cylinders from initial_pos to the end
        for (int i = idx; i < num; i++) {
            head_movement += abs(temp_requests[i] - current_pos);
            current_pos = temp_requests[i];
            
            // Record this request
            result_order[*order_count].cylinder = current_pos;
            result_order[*order_count].is_endpoint = (current_pos == 0 || current_pos == total_cylinders - 1);
            result_order[*order_count].is_original = !(current_pos == 0 || current_pos == total_cylinders - 1);
            (*order_count)++;
        }
        
        // Then go back and serve cylinders from initial_pos to the beginning
        for (int i = idx - 1; i >= 0; i--) {
            head_movement += abs(current_pos - temp_requests[i]);
            current_pos = temp_requests[i];
            
            // Record this request
            result_order[*order_count].cylinder = current_pos;
            result_order[*order_count].is_endpoint = (current_pos == 0 || current_pos == total_cylinders - 1);
            result_order[*order_count].is_original = !(current_pos == 0 || current_pos == total_cylinders - 1);
            (*order_count)++;
        }
    } else {  // direction == 'd' or direction == 'D'
        // First serve all cylinders from initial_pos to the beginning
        for (int i = idx - 1; i >= 0; i--) {
            head_movement += abs(current_pos - temp_requests[i]);
            current_pos = temp_requests[i];
            
            // Record this request
            result_order[*order_count].cylinder = current_pos;
            result_order[*order_count].is_endpoint = (current_pos == 0 || current_pos == total_cylinders - 1);
            result_order[*order_count].is_original = !(current_pos == 0 || current_pos == total_cylinders - 1);
            (*order_count)++;
        }
        
        // Then go up and serve cylinders from initial_pos to the end
        for (int i = idx; i < num; i++) {
            head_movement += abs(temp_requests[i] - current_pos);
            current_pos = temp_requests[i];
            
            // Record this request
            result_order[*order_count].cylinder = current_pos;
            result_order[*order_count].is_endpoint = (current_pos == 0 || current_pos == total_cylinders - 1);
            result_order[*order_count].is_original = !(current_pos == 0 || current_pos == total_cylinders - 1);
            (*order_count)++;
        }
    }
    
    free(temp_requests);
    return head_movement;
}

// C-SCAN algorithm (Circular SCAN)
int c_scan(int requests[], int num, int initial_pos, int total_cylinders, char direction, Request result_order[], int *order_count) {
    int head_movement = 0;
    int current_pos = initial_pos;
    int *temp_requests;
    
    *order_count = 0;
    
    // Make a copy of the requests, adding both boundaries
    temp_requests = (int*)malloc((num + 2) * sizeof(int)); // +2 for boundaries
    if (temp_requests == NULL) {
        printf("Memory allocation failed.\n");
        return -1;
    }
    
    // Add boundary requests (0 and total_cylinders - 1)
    temp_requests[0] = 0;
    temp_requests[1] = total_cylinders - 1;
    for (int i = 0; i < num; i++) {
        temp_requests[i + 2] = requests[i];
    }
    num += 2; // Update number of requests to include boundaries
    
    // Sort all requests, including boundaries
    bubbleSort(temp_requests, num);
    
    // Find position where requests are >= initial_pos
    int idx = 0;
    while (idx < num && temp_requests[idx] < initial_pos) {
        idx++;
    }
    
    if (direction == 'u' || direction == 'U') {
        // First serve all cylinders from initial_pos to the end
        for (int i = idx; i < num; i++) {
            head_movement += abs(temp_requests[i] - current_pos);
            current_pos = temp_requests[i];
            
            // Record this request
            result_order[*order_count].cylinder = current_pos;
            result_order[*order_count].is_endpoint = (current_pos == 0 || current_pos == total_cylinders - 1);
            result_order[*order_count].is_original = !(current_pos == 0 || current_pos == total_cylinders - 1);
            (*order_count)++;
        }
        
        // Jump to the beginning and serve cylinders up to initial_pos
        current_pos = 0;
        
        // Record jump to beginning
        result_order[*order_count].cylinder = current_pos;
        result_order[*order_count].is_endpoint = 1;
        result_order[*order_count].is_original = 0;
        (*order_count)++;
        
        // Serve remaining requests
        for (int i = 0; i < idx; i++) {
            head_movement += abs(temp_requests[i] - current_pos);
            current_pos = temp_requests[i];
            
            // Record this request
            result_order[*order_count].cylinder = current_pos;
            result_order[*order_count].is_endpoint = (current_pos == 0 || current_pos == total_cylinders - 1);
            result_order[*order_count].is_original = !(current_pos == 0 || current_pos == total_cylinders - 1);
            (*order_count)++;
        }
    } else {  // direction == 'd' or direction == 'D'
        // First serve all cylinders from initial_pos to the beginning
        for (int i = idx - 1; i >= 0; i--) {
            head_movement += abs(current_pos - temp_requests[i]);
            current_pos = temp_requests[i];
            
            // Record this request
            result_order[*order_count].cylinder = current_pos;
            result_order[*order_count].is_endpoint = (current_pos == 0 || current_pos == total_cylinders - 1);
            result_order[*order_count].is_original = !(current_pos == 0 || current_pos == total_cylinders - 1);
            (*order_count)++;
        }
        
        // Jump to the end and serve cylinders down to initial_pos
        current_pos = total_cylinders - 1;
        
        // Record jump to end
        result_order[*order_count].cylinder = current_pos;
        result_order[*order_count].is_endpoint = 1;
        result_order[*order_count].is_original = 0;
        (*order_count)++;
        
        // Serve remaining requests in descending order
        for (int i = num - 1; i >= idx; i--) {
            head_movement += abs(current_pos - temp_requests[i]);
            current_pos = temp_requests[i];
            
            // Record this request
            result_order[*order_count].cylinder = current_pos;
            result_order[*order_count].is_endpoint = (current_pos == 0 || current_pos == total_cylinders - 1);
            result_order[*order_count].is_original = !(current_pos == 0 || current_pos == total_cylinders - 1);
            (*order_count)++;
        }
    }
    
    free(temp_requests);
    return head_movement;
}

// Helper function for bubble sort
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Simple bubble sort implementation
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}
