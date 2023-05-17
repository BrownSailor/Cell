#include <stdio.h>

// ; Problem 7
// ; print the 10001st prime number
// 
// fn solve: nil -> nil {
//     N: 10001
// 
//     primes: 10001 * num
//     primes[1]: 2
// 
//     i: 2
//     @ i <= N {
//         cur_prime: primes[i - 1] + 1
//         j: i - 1
//         @ j > 0 {
//             ? cur_prime % primes[j] = 0 {
//                 cur_prime: cur_prime + 1
//                 j: i
//             }
//             j: j - 1
//         }
//         primes[i]: cur_prime
//         i: i + 1
//     }
// 
//     println primes[N]
// }
// 
// solve

void solve()
{
    const long long N = 10001;
    long long primes[10001];
    primes[0] = 2;

    for (int i = 1; i < N; i++)
    {
        long long cur_prime = primes[i - 1] + 1;
        for (int j = i - 1; j >= 0; j--)
        {
            if (cur_prime % primes[j] == 0)
            {
                cur_prime = cur_prime + 1;
                j = i;
            }
        }

        primes[i] = cur_prime;
    }

    printf("%lld\n", primes[N - 1]);
}

int main()
{
    solve();
    return 0;
}
