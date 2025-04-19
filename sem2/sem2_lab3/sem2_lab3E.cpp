#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

struct point {
    int cost;
    int k1;
    int k2;
    vector<int> daysofkupons;
};

int main() {
    int n;
    cin >> n;
    vector<int> pricelist(n + 1, 0);
    for (int i = 1; i < n + 1; i++) {
        cin >> pricelist[i];
    }

    vector<vector<point>> mincost(n + 1, vector<point>(n + 2, {INT_MAX, 0, 0, {}}));
    mincost[0][0] = {0, 0, 0, {}};

    if (n == 0) {
        cout << 0 << endl << 0 << " " << 0;
        return 0;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            if (mincost[i-1][j].cost == INT_MAX) continue;

            //покупаем без купона
            point new_state = mincost[i-1][j];
            new_state.cost += pricelist[i];
            int new_j = j;
            if (pricelist[i] > 100) {
                new_j++;
                new_state.k1++;
            }
            if (new_state.cost < mincost[i][new_j].cost) {
                mincost[i][new_j] = new_state;
            }

            //используем купон (если есть купон)
            if (j > 0) {
                point use_coupon = mincost[i-1][j];
                use_coupon.cost += 0;
                use_coupon.k2++;
                use_coupon.daysofkupons.push_back(i);
                if (use_coupon.cost < mincost[i][j-1].cost) {
                    mincost[i][j-1] = use_coupon;
                }
            }
        }
    }

    // Находим лучший результат
    point best = {INT_MAX, 0, 0, {}};
    int remaining = 0;
    for (int j = 0; j <= n; j++) {
        if (mincost[n][j].cost < best.cost) {
            best = mincost[n][j];
            remaining = j;
        } else if (mincost[n][j].cost == best.cost && j > remaining) {
            best = mincost[n][j];
            remaining = j;
        }
    }

    // Вывод результата
    cout << best.cost << endl;
    cout << best.k1 - best.k2 << " " << best.k2 << endl;
    sort(best.daysofkupons.begin(), best.daysofkupons.end());
    for (int day : best.daysofkupons) {
        cout << day << endl;
    }

    return 0;
}