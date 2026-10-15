#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft;

uint16_t calibration[5] = {
    285, 3603, 274, 3556, 7};

const char *commands[] = {
    "Format", "Lint", "Typecheck", "Test"};

int selectedCommand = -1;
bool wasTouching = false;

void drawButtons()
{
  for (int i = 0; i < 4; i++)
  {
    int x = 30 + (i % 2) * 220;
    int y = 70 + (i / 2) * 100;

    uint16_t color =
        selectedCommand == i ? TFT_GREEN : TFT_BLUE;

    tft.fillRoundRect(x, y, 200, 80, 10, color);
    tft.setTextColor(TFT_WHITE, color);
    tft.drawCentreString(commands[i], x + 100, y + 25, 4);
  }
}

void setup()
{
  Serial.begin(115200);

  tft.init();
  tft.setRotation(1);
  tft.setTouch(calibration);
  tft.fillScreen(TFT_BLACK);

  tft.drawCentreString("Developer Command Center", 240, 20, 2);

  drawButtons();
}

void loop()
{
  uint16_t x, y;

  bool touching =
      tft.getTouchRawZ() > 300 &&
      tft.getTouch(&x, &y);

  if (!touching)
  {
    wasTouching = false;
    delay(30);
    return;
  }

  if (wasTouching)
    return;
  wasTouching = true;

  for (int i = 0; i < 4; i++)
  {
    int bx = 30 + (i % 2) * 220;
    int by = 70 + (i / 2) * 100;

    if (x >= bx && x < bx + 200 &&
        y >= by && y < by + 80)
    {

      if (selectedCommand != i)
      {
        selectedCommand = i;
        drawButtons();

        // Send command to MacBook
        Serial.println(commands[i]);
      }

      break;
    }
  }
}