#include "b2.h" // Include the corresponding header file

// Initialize the static member
std::vector<B2*> B2::b2;

// Default constructor implementation
B2::B2() {
    b2.push_back(this); // Add this instance to the static vector
    type = B2plane; // Assign default type
    speed = 0.0f; // Initialize speed
}

// Destructor implementation
B2::~B2() {
    // Remove this instance from the static vector
    auto it = std::find(b2.begin(), b2.end(), this);
    if (it != b2.end()) {
        b2.erase(it); // Remove if found
    }
}

// Static function to render all B2 entities
void B2::renderAll(Camera* camera) {
    for (B2* entity : b2) {
        if (entity) {
            // Code to render the entity goes here
            // Example:
            entity->render(camera);
        }
    }
}

// Static function to update all B2 entities
void B2::updateAll(float seconds_elapsed) {
    for (B2* entity : b2) {
        if (entity) {
            // Code to update the entity based on elapsed time
            // Example:
            entity->speed += 0.1f * seconds_elapsed; // Increase speed
            // Other update logic
        }
    }
}

// Function to render all B2 entities (external prototype)
void renderB2() {
    B2::renderAll(); // Simply call the static render function
}

// External active B2 count
int active_b2 = 0; // Initialize the active B2 count
