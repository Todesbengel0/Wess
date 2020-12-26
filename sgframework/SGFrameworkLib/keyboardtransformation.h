/*
 *  KeyboardTransformation.h
 *  robot
 *
 *  Created by bdr on 12.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#ifndef _KeyboardTransformation_H_
#define _KeyboardTransformation_H_
#include "transformation.h"
#include "listener.h"

//! Diese Klasse verbindet Transformationen mit Tastatureingaben

//! Durch die Verwendung der Methoden ist es sehr einfach möglich eine Transformation einzufügen,
//! die über Tastatureingaben gesteuert werden kann.
class KeyboardTransformation : public Transformation, public Listener
{
public:
    KeyboardTransformation();

    //! Ist für die Translationskeys ein Modifier (ALT, ...) erforderlich?
    void setTransModifier(int s);

    //! Ist für die Rotationskeys ein Modifier (ALT, ...) erforderlich?
    void setRotModifier(int s);

    //! Setzen der Translationskeys, wobei die Umkehrrichtung immer der passende Großbuchstabe ist
    void setTransKeysUpper(int x, int y, int z);

    //! Setzen der Keys für die Translation
    void setTransKeys(int x1, int x2, int y1, int y2, int z1, int z2);

    //! Setzen der Rotkeys, wobei die Umkehrrichtung immer der passende Großbuchstabe ist
    void setRotKeysUpper(int x, int y, int z);

    //! Setzen des Keys für die Rotation
    void setRotKeys(int x1, int x2, int y1, int y2, int z1, int z2);

    //! Wie schnell (also um welchen Offset) wird jeweils bei Tastendruck verschoben?
    void setTransspeed(float s);

    //! Wie schnell (also um welchen Winkel) wird jeweils bei Tastendruck rotiert
    void setRotspeed(float s);

    //! Keyboard Callback ohne Modifier, mappen des Keys auf Aktion
    virtual void keyboard(int key, int modifier) override;

    //! Bezeichner für Keys, die man nicht belegen will (Parameter für SetRotKeys, etc.)
    static const int NoKey;

private:
    int mTranskeys[6];
    int mTransmodifier;
    bool mTransmodifierUsed;
    int mRotkeys[6];
    int mRotmodifier;
    bool mRotmodifierUsed;
    float mAngle;  // Winkel, um den bei Eingabe weitergedreht wird
    float mOffset; // Offset, um den jeweils verschoben wird
    void selectandActTrans(int key, int modifier);
    void selectandActRot(int key, int modifier);
};

#endif
