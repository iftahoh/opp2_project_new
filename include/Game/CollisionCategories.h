#pragma once

// נשתמש ב-short כדי לחסוך מקום, ונגדיר את הקטגוריות באמצעות הזזת ביטים.
// זה מבטיח שכל קטגוריה היא חזקה ייחודית של 2.
constexpr short CATEGORY_TERRAIN = 0x0001; // ...00000001
constexpr short CATEGORY_WORM = 0x0002; // ...00000010
// בעתיד תוכל להוסיף עוד, למשל:
// constexpr short CATEGORY_PROJECTILE = 0x0004; // ...00000100