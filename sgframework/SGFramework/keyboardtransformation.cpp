/*
 *  KeyboardTrans.cpp
 *  robot
 *
 *  Created by bdr on 12.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#include "keyboardtransformation.h"

const int KeyboardTransformation::NoKey = -1;

KeyboardTransformation::KeyboardTransformation() : Transformation()
{
    mTransmodifierUsed = false;
    mRotmodifierUsed = false;
    setRotKeys(-1, -1, -1, -1, -1, -1);
    setTransKeys(-1, -1, -1, -1, -1, -1);
    mAngle = 1.0f;
    mOffset = 0.3f;
}

//! Umso höher der Wert s, umso schneller verschiebt sich das jeweilige Objekt.
//! Da keine weiteren Timing-Klassen implementiert sind, ist das eine einfache
//! Möglichkeit die Geschwindigkeit zu beeinflussen.
void KeyboardTransformation::setTransspeed(float s)
{
    mOffset = s;
}

//! Siehe SetTransspeed
void KeyboardTransformation::setRotspeed(float s)
{
    mAngle = s;
}

//! Angabe aller Codes zum steuern. Übergeben werden in der Regel Buchstaben wie 'a'.
//! Die Parameter sind int, da damit auch die Specialkeys von GLUT behandelt werden
//! können, z.B. ALT-XXX. Siehe hierzu auch SetRotModifier und SetTransModifier
//!\param x1 Verschieben in positiver x-Richtung
//!\param x2 Verschieben in negativer x-Richtung
//!\param y1 Verschieben in positiver y-Richtung
//!\param y2 Verschieben in negativer y-Richtung
//!\param z1 Verschieben in positiver z-Richtung
//!\param z2 Verschieben in negativer z-Richtung
void KeyboardTransformation::setTransKeys(int x1, int x2, int y1, int y2, int z1, int z2)
{
    mTranskeys[0] = x1;
    mTranskeys[1] = x2;
    mTranskeys[2] = y1;
    mTranskeys[3] = y2;
    mTranskeys[4] = z1;
    mTranskeys[5] = z2;
}

//! Angabe einfacher Buchstaben zum steuern, Großbuchstabe ist Gegenrichtung.
//! D.h. SetTransKeysUpper('a','b','c') entspricht dem Aufruf
//! SetTransKeys('a','A','b','B','c','C').
void KeyboardTransformation::setTransKeysUpper(int x, int y, int z)
{
    setTransKeys(x, toupper(x), y, toupper(y), z, toupper(z));
}

//! Angabe aller Codes zum steuern
//!\param x1 Key, um mathematisch positiv um die X-Achse zu drehen
//!\param x2 Key, um mathematisch negativ um die X-Achse zu drehen
//!\param y1 Key, um mathematisch positiv um die y-Achse zu drehen
//!\param y2 Key, um mathematisch negativ um die y-Achse zu drehen
//!\param z1 Key, um mathematisch positiv um die z-Achse zu drehen
//!\param z2 Key, um mathematisch negativ um die z-Achse zu drehen
void KeyboardTransformation::setRotKeys(int x1, int x2, int y1, int y2, int z1, int z2)
{
    mRotkeys[0] = x1;
    mRotkeys[1] = x2;
    mRotkeys[2] = y1;
    mRotkeys[3] = y2;
    mRotkeys[4] = z1;
    mRotkeys[5] = z2;
}

//! Angabe einfacher Buchstaben zum steuern, Großbuchstabe ist Gegenrichtung
void KeyboardTransformation::setRotKeysUpper(int x, int y, int z)
{
    setRotKeys(x, toupper(x), y, toupper(y), z, toupper(z));
}

void KeyboardTransformation::setRotModifier(int s)
{
    mRotmodifier = s;
    mRotmodifierUsed = true;
}

void KeyboardTransformation::selectandActTrans(int key, int modifier)
{
    if (!mTransmodifierUsed || (mTransmodifierUsed && (modifier == mTransmodifier)))
    {
        if (key == mTranskeys[0])
        {
            this->translate(mOffset, 0.0, 0.0);
        }
        else if (key == mTranskeys[1])
        {
            this->translate(-mOffset, 0.0, 0.0);
        }
        else if (key == mTranskeys[2])
        {
            this->translate(0.0, mOffset, 0.0);
        }
        else if (key == mTranskeys[3])
        {
            this->translate(0.0, -mOffset, 0.0);
        }
        else if (key == mTranskeys[4])
        {
            this->translate(0.0, 0.0, mOffset);
        }
        else if (key == mTranskeys[5])
        {
            this->translate(0.0, 0.0, -mOffset);
        }
    }
}

void KeyboardTransformation::selectandActRot(int key, int modifier)
{
    if (!mRotmodifierUsed || (mRotmodifierUsed && (modifier == mRotmodifier)))
    {
        if (key == mRotkeys[0])
        {
            this->rotate(mAngle, 1.0, 0.0, 0.0);
        }
        else if (key == mRotkeys[1])
        {
            this->rotate(-mAngle, 1.0, 0.0, 0.0);
        }
        else if (key == mRotkeys[2])
        {
            this->rotate(mAngle, 0.0, 1.0, 0.0);
        }
        else if (key == mRotkeys[3])
        {
            this->rotate(-mAngle, 0.0, 1.0, 0.0);
        }
        else if (key == mRotkeys[4])
        {
            this->rotate(mAngle, 0.0, 0.0, 1.0);
        }
        else if (key == mRotkeys[5])
        {
            this->rotate(-mAngle, 0.0, 0.0, 1.0);
        }
    }
}

void KeyboardTransformation::keyboard(int key, int modifier)
{
    selectandActTrans((int)key, modifier);
    selectandActRot((int)key, modifier);
}
