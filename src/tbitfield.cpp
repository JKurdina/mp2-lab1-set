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
    if (len < 0)
        throw - 1;
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
    if (n > BitLen || n < 0)
        throw out_of_range("bit pos is out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n > BitLen || n < 0)
        throw out_of_range("bit pos is out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);

}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n > BitLen || n < 0)
        throw out_of_range("bit pos is out of range");
    return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;
    else
    {
        BitLen = bf.BitLen;
        if (MemLen != bf.MemLen) {
            TELEM* p = new TELEM[bf.MemLen];
            
            delete[] pMem;
            MemLen = bf.MemLen;
            pMem = p;
        }
        copy(bf.pMem, bf.pMem + MemLen, pMem);
        return *this;
    }
 
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
    return (!(*this == bf));
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int len = max(BitLen, bf.BitLen);
    TBitField tmp(len);
    for (int i = 0; i < MemLen; i++)
        tmp.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        tmp.pMem[i] = tmp.pMem[i] | bf.pMem[i];
    return tmp;

}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len = max(BitLen, bf.BitLen);
    TBitField tmp(len);
    for (int i = 0; i < MemLen; i++)
        tmp.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        tmp.pMem[i] = tmp.pMem[i] & bf.pMem[i];
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(BitLen);
    int i = 0;
    for (i = 0; i < MemLen; i++)
        tmp.pMem[i] = ~pMem[i];

    int mask = BitLen & ((1 << 5) - 1);
    
    tmp.pMem[i - 1] = tmp.pMem[i - 1] & ((1 << mask) - 1);
    return tmp;
    
    
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int len;
    istr >> len;
    TBitField tmp(len);
    char* buf = new char[len];
    istr >> buf;
    for (int i = 0; i < bf.BitLen; i++)
        if (buf[i] == 0)
            bf.ClrBit(i);
        else
            bf.SetBit(i);

    return istr;
    
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
