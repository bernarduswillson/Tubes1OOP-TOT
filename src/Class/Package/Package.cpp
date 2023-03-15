#include "Package.hpp"
#include <cmath>
#include <bits/stdc++.h>

Package::Package():Calculable()
{
    this->cardBuffer = std::vector<Card>();
}

Package::~Package()
{
}

double Package::getValue()
{
    return value;
}

std::vector<Card> Package::getCardBuffer()
{
    return cardBuffer;
}

void Package::addCard(Card card)
{
    cardBuffer.push_back(card);
}

void Package::setValue(double value)
{
    this->value = value;
}

void Package::operator=(const Package &other)
{
    this->cardBuffer = other.cardBuffer;
    this->value = other.value;
}
