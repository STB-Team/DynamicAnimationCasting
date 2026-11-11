#include "ResourceCost.h"
#include <cstdio>

using Loki::AnimationCasting::CheckResourceCost;
using Loki::AnimationCasting::ResourceCostResult;

int main() {
    struct Case {
        const char* name;
        float available;
        float required;
        bool player;
        ResourceCostResult expected;
    };
    const Case cases[] = {
        {"negative player value, free marker", -5.f, 0.f, true, ResourceCostResult::affordable},
        {"negative NPC value, free marker", -5.f, 0.f, false, ResourceCostResult::affordable},
        {"zero value, free marker", 0.f, 0.f, true, ResourceCostResult::affordable},
        {"player shortage flashes", 4.f, 5.f, true, ResourceCostResult::insufficientPlayer},
        {"NPC shortage rejects silently", 4.f, 5.f, false, ResourceCostResult::insufficient},
        {"exact budget", 5.f, 5.f, true, ResourceCostResult::affordable},
        {"sufficient NPC budget", 10.f, 5.f, false, ResourceCostResult::affordable},
        {"negative value, refund", -10.f, -5.f, true, ResourceCostResult::affordable},
    };
    for (const auto& c : cases) {
        if (CheckResourceCost(c.available, c.required, c.player) != c.expected) {
            std::fprintf(stderr, "FAIL: %s\n", c.name);
            return 1;
        }
    }
    std::puts("ResourceCost: 8 cases passed");
}
