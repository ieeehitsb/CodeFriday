#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ============================================================
   SHA-256 IMPLEMENTATION (PUBLIC DOMAIN)
   ============================================================ */

typedef struct {
    uint32_t state[8];
    uint32_t count[2];
    unsigned char buffer[64];
} SHA256_CTX;

#define ROTLEFT(a,b)  (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z)  (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x)  (ROTRIGHT(x,2)  ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x)  (ROTRIGHT(x,6)  ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7)  ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

static const uint32_t k[64] = {
  0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
  0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
  0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
  0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
  0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
  0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
  0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
  0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

void sha256_transform(SHA256_CTX *ctx, const unsigned char data[]) {
    uint32_t a, b, c, d, e, f, g, h, t1, t2, m[64];
    for (int i = 0, j = 0; i < 16; i++, j += 4)
        m[i] = (data[j] << 24) | (data[j+1] << 16) |
               (data[j+2] << 8) | (data[j+3]);

    for (int i = 16; i < 64; i++)
        m[i] = SIG1(m[i-2]) + m[i-7] + SIG0(m[i-15]) + m[i-16];

    a = ctx->state[0]; b = ctx->state[1]; c = ctx->state[2]; d = ctx->state[3];
    e = ctx->state[4]; f = ctx->state[5]; g = ctx->state[6]; h = ctx->state[7];

    for (int i = 0; i < 64; i++) {
        t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
        t2 = EP0(a) + MAJ(a,b,c);
        h = g; g = f; f = e; 
        e = d + t1;
        d = c; c = b; b = a;
        a = t1 + t2;
    }

    ctx->state[0] += a; ctx->state[1] += b; ctx->state[2] += c; ctx->state[3] += d;
    ctx->state[4] += e; ctx->state[5] += f; ctx->state[6] += g; ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx) {
    ctx->count[0] = ctx->count[1] = 0;
    ctx->state[0] = 0x6a09e667; ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372; ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f; ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab; ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const unsigned char data[], size_t len) {
    uint32_t i = ctx->count[0] >> 3;
    ctx->count[0] += len << 3;
    if (ctx->count[0] < (len << 3)) ctx->count[1]++;

    size_t fill = 64 - (i & 0x3F);

    if (i & 0x3F) {
        if (len >= fill) {
            memcpy(ctx->buffer + (i & 0x3F), data, fill);
            sha256_transform(ctx, ctx->buffer);
            data += fill;
            len -= fill;
        } else {
            memcpy(ctx->buffer + (i & 0x3F), data, len);
            return;
        }
    }

    while (len >= 64) {
        sha256_transform(ctx, data);
        data += 64;
        len -= 64;
    }

    memcpy(ctx->buffer, data, len);
}

void sha256_final(SHA256_CTX *ctx, unsigned char hash[]) {
    unsigned char pad[64] = {0x80};
    unsigned char len[8];

    for (int i = 0; i < 8; i++)
        len[7 - i] = (ctx->count[((i >= 4) ? 0 : 1)] >> ((i % 4) * 8)) & 0xFF;

    size_t pad_len = 64 - ((ctx->count[0] >> 3) & 0x3F);
    if (pad_len < 9) pad_len += 64;

    sha256_update(ctx, pad, pad_len - 8);
    sha256_update(ctx, len, 8);

    for (int i = 0; i < 8; i++) {
        hash[i*4    ] = (ctx->state[i] >> 24) & 0xFF;
        hash[i*4 + 1] = (ctx->state[i] >> 16) & 0xFF;
        hash[i*4 + 2] = (ctx->state[i] >> 8)  & 0xFF;
        hash[i*4 + 3] = (ctx->state[i])       & 0xFF;
    }
}

/* ============================================================
   HELPER: Compute SHA-256 Hex String
   ============================================================ */

void sha256_hex(const char *data, char out[65]) {
    unsigned char hash[32];
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, (unsigned char*)data, strlen(data));
    sha256_final(&ctx, hash);

    for (int i = 0; i < 32; i++)
        sprintf(out + (i*2), "%02x", hash[i]);

    out[64] = '\0';
}

/* ============================================================
   MERKLE TREE IMPLEMENTATION
   ============================================================ */

void compute_merkle_root(char *items[], int n, char root[65]) {
    int count = n;
    char **hashes = malloc(sizeof(char*) * count);

    // Hash each leaf node
    for (int i = 0; i < count; i++) {
        hashes[i] = malloc(65);
        sha256_hex(items[i], hashes[i]);
    }

    // Build tree
    while (count > 1) {
        int next = (count + 1) / 2;
        char **new_hashes = malloc(sizeof(char*) * next);

        for (int i = 0; i < next; i++) {
            char concat[200] = {0};

            if (i*2 + 1 < count) {
                sprintf(concat, "%s%s", hashes[i*2], hashes[i*2 + 1]);
            } else {
                sprintf(concat, "%s%s", hashes[i*2], hashes[i*2]); // duplicate last
            }

            new_hashes[i] = malloc(65);
            sha256_hex(concat, new_hashes[i]);
        }

        for (int i = 0; i < count; i++)
            free(hashes[i]);
        free(hashes);

        hashes = new_hashes;
        count = next;
    }

    strcpy(root, hashes[0]);
    free(hashes[0]);
    free(hashes);
}

/* ============================================================
   DRIVER CODE (MATCHES YOUR FORMAT)
   ============================================================ */

int main() {
    char *data[] = {"A", "B", "C", "D"};
    int n = 4;

    char root[65];
    compute_merkle_root(data, n, root);

    printf("Merkle Root: %s\n", root);
    return 0;
}
