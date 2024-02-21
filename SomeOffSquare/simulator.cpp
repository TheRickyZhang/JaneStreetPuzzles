#include <iostream>
#include <cmath>
#include <random>
using namespace std;

// Generate points
int main() {
    random_device rd;
    mt19937  gen(rd());
    uniform_real_distribution<> distr(0, 1);

    int success = 0;
    double trials = 10000000;
    for(int i = 0; i < trials; ++i){
        double x1 = distr(gen);
        double y1 = distr(gen);
        double x2 = distr(gen);
        double y2 = distr(gen);
        double midX = (x1 + x2)/2;
        double midY = (y1 + y2)/2;
        double rad = sqrt(pow(x1-x2, 2) + pow(y1-y2, 2))/2;
        if(midX + midY < 1 && midX > midY) {
            if (midY - rad > 0) {
                success++;
            }
        }
        else if(midX + midY < 1 && midX < midY) {
            if (midX - rad > 0) {
                success++;
            }
        }
        if(midX + midY > 1 && midX > midY) {
            if (midY + rad < 1) {
                success++;
            }
        }
        else if(midX + midY > 1 && midX < midY) {
            if (midX + rad < 1) {
                success++;
            }
        }
    }
    cout << success/trials;
    return 0;
}