// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    BitLen = len;
    MemLen = (BitLen + sizeof(int) * 8 - 1) / (sizeof(int) * 8);
    pMem = new TELEM[MemLen];
    memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & 31);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= BitLen)
        out_of_range("bit pos is out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= BitLen)
        throw out_of_range("bit pos is out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);

}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n >= BitLen)
        throw out_of_range("bit pos is out of range");
    return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen)
    {
        TELEM* p = new TELEM[bf.MemLen];
        MemLen = bf.MemLen;
        delete[] pMem;
        pMem = p;
    }
    memcpy(pMem, bf.pMem, MemLen);
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (!(*this == bf))
        return 1;
    return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (BitLen != bf.BitLen)
        throw length_error("TBitField objects have different size");
    TBitField tmp(*this);
    for (int i = 0; i < bf.MemLen; i++) {
        tmp.pMem[i] |= bf.pMem[i];
    }
    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (BitLen != bf.BitLen)
        throw length_error("TBitField objects have different size");
    TBitField tmp(*this);
    for (int i = 0; i < bf.MemLen; i++) {
        tmp.pMem[i] &= bf.pMem[i];
    }
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen);
    for (int i = 0; i < MemLen; i++) {
        temp.pMem[i] = ~pMem[i];
    }
    return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
//   int len = bf.GetLength();
//   for (int i = 0; i < len; i++)
//       if (bf.GetBit(i) == 1)
//           istr >> '1';
//       else
//           istr >> '0';
//   return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    int len = bf.GetLength();
    for (int i = 0; i < len; i++)
        if (bf.GetBit(i) == 1)
            ostr << '1';
        else
            ostr << '0';
    return ostr;
}
