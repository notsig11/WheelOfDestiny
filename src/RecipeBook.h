#pragma once
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <Wire.h>

#include "i2c_proto.h"

using Controller = uint8_t;
using Pump = uint8_t;
using LiquorMap = std::unordered_map<std::string, std::pair<Controller, Pump>>;

// Just a container class for now... Maybe do something fancy with substitutes in the future.
struct Recipe {
    const std::string name;
    const std::vector<std::string> ingredients;
    Recipe(std::string name, std::vector<std::string> ingredients) : name(std::move(name)), ingredients(std::move(ingredients)) {};

};

class RecipeBook {
    const std::vector<Recipe> recipes {
        { "Mini Long Island", { "vodka",  "tequila", "gin" } },
        { "Gasoline",         { "tequila", "vodka", "gin", "peppermint schnaupps" } } ,
        { "Oil Spill",        { "curacao", "jager", "fireball", "cranberry" } },
        { "kamikaze",         { "vodka", "triple sec", "lime" } },
        { "liquid cocaine",   { "jager", "151", "peppermint schnaupps" } },
        { "Flamin Beaver",    { "fireball", "cider" } },
        { "Woo Woo",          { "vodka", "cranberry", "peach schnaupps" } },
        { "Polar Bear",       { "creme de cacao", "peppermint schnaupps" } },
        { "Red-headed Slut",  { "jager", "peach schnuapps", "cranberry" } },
        { "Washington Apple", { "crown", "apple pucker", "cranberry" } },
        { "lemon drop",       { "vodka", "melon", "simple syrup" } },
        { "3 Way With A Dirty Pirate", { "spiced rum", "coconut rum", "white rum" } }
    };

    const LiquorMap liquorMap { { "vodka", {PUMP_CONTROL_0, 0} },
                                { "jager", {PUMP_CONTROL_0, 1} },
                                { "peppermint schnaupps", {PUMP_CONTROL_0, 2} },
                                { "triple sec", {PUMP_CONTROL_0, 3} },

                                { "tequila", {PUMP_CONTROL_1, 0} },
                                { "creme de cacao", {PUMP_CONTROL_1, 1} },
                                { "crown", {PUMP_CONTROL_1, 2} },
                                { "melon", {PUMP_CONTROL_1, 3} },

                                { "cranberry", {PUMP_CONTROL_2, 0} },
                                { "simple syrup", {PUMP_CONTROL_2, 1} },
                                { "gin", {PUMP_CONTROL_2, 2} },
                                { "coconut rum", {PUMP_CONTROL_2, 3} },

                                { "apple pucker", {PUMP_CONTROL_3, 0} },
                                { "white rum", {PUMP_CONTROL_3, 1} },
                                { "spiced rum", {PUMP_CONTROL_3, 2} },
                                { "peach schnaupps", {PUMP_CONTROL_3, 3} },

                                { "fireball", {PUMP_CONTROL_4, 0} },
                                { "cider", {PUMP_CONTROL_4, 1} },
                                { "151", {PUMP_CONTROL_4, 2} },
                                { "lime", {PUMP_CONTROL_4, 3} },

                                { "curacao", {PUMP_CONTROL_5, 0} },
                                { "vodka", {PUMP_CONTROL_5, 1} },
                                { "vodka", {PUMP_CONTROL_5, 2} },
                                { "vodka", {PUMP_CONTROL_5, 3} },
    };

    TwoWire* i2cBus;
public:
    explicit RecipeBook(TwoWire* wire) : i2cBus(wire) {};
    bool mixRecipe(std::string_view drink) const;

    std::optional<Recipe> getRecipeAtIndex(int index) const;

};
