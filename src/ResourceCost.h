#pragma once

namespace Loki::AnimationCasting {
    enum class ResourceCostResult { affordable, insufficient, insufficientPlayer };

    // Free/refunding events must work even when an actor value is below zero.
    constexpr ResourceCostResult CheckResourceCost(float available, float required, bool isPlayer) noexcept {
        if (required > 0.f && available < required) {
            return isPlayer ? ResourceCostResult::insufficientPlayer : ResourceCostResult::insufficient;
        }
        return ResourceCostResult::affordable;
    }
}
