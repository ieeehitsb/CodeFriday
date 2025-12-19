#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const char base32[] = "0123456789bcdefghjkmnpqrstuvwxyz";

// Encode latitude & longitude into Geohash
char* geohash_encode(double lat, double lon, int precision) {
    double lat_range[2] = {-90.0, 90.0};
    double lon_range[2] = {-180.0, 180.0};

    bool is_even = true;
    int bit = 0, ch = 0;
    int idx = 0;

    char* geohash = (char*)malloc(precision + 1);

    while (idx < precision) {
        double mid;
        if (is_even) {
            mid = (lon_range[0] + lon_range[1]) / 2;
            if (lon >= mid) {
                ch |= (1 << (4 - bit));
                lon_range[0] = mid;
            } else {
                lon_range[1] = mid;
            }
        } else {
            mid = (lat_range[0] + lat_range[1]) / 2;
            if (lat >= mid) {
                ch |= (1 << (4 - bit));
                lat_range[0] = mid;
            } else {
                lat_range[1] = mid;
            }
        }

        is_even = !is_even;

        if (bit < 4) {
            bit++;
        } else {
            geohash[idx++] = base32[ch];
            bit = 0;
            ch = 0;
        }
    }

    geohash[idx] = '\0';
    return geohash;
}

// ---------------- Driver Code ----------------
int main() {
    double latitude = 37.7749;
    double longitude = -122.4194;

    char* hash = geohash_encode(latitude, longitude, 6);
    printf("Geohash: %s\n", hash);

    free(hash);
    return 0;
}
