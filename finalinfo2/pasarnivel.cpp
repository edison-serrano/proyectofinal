#include "pasarnivel.h"

pasarnivel::pasarnivel(int x, int y, int width, int height)
{
    setRect(x, y, width, height);
    setBrush(QBrush(Qt::red));
}
