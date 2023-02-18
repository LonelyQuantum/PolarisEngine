#pragma once

#define POLARIS_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define POLARIS_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define POLARIS_PIN(a, min_value, max_value) POLARIS_MIN(max_value, POLARIS_MAX(a, min_value))

#define POLARIS_VALID_INDEX(idx, range) (((idx) >= 0) && ((idx) < (range)))
#define POLARIS_PIN_INDEX(idx, range) POLARIS_PIN(idx, 0, (range)-1)

#define POLARIS_SIGN(x) ((((x) > 0.0f) ? 1.0f : 0.0f) + (((x) < 0.0f) ? -1.0f : 0.0f))
