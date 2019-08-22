#include <Average.h>

// Reserve space for 10 entries in the average bucket.
// Change the type between < and > to change the entire way the library works.
Average<int> ave(10);

void setup() {
    Serial.begin(9600);
}

void loop() {
    
    ave.push(1);
    ave.push(2);
    ave.push(3);
    ave.push(2);
    ave.push(2);
    ave.push(2);

    // Display the current data set
    for (int i = 0; i < 10; i++) {
        Serial.print(ave.get(i));
        Serial.print(" ");
    }
    Serial.println();

    // And show some interesting results.
    Serial.print("Mode:   "); Serial.println(ave.mode());
    delay(1000);
}
