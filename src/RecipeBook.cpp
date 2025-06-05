#include "RecipeBook.h"
#include <HardwareSerial.h>
#include <algorithm>

bool RecipeBook::mixRecipe(std::string_view drink) const {
    const auto recipe = std::find_if(recipes.begin(), recipes.end(), [drink](const Recipe& r) { return drink == r.name; });
    if (recipe == recipes.end()) {
        Serial.printf("Didn't find recipe %s\n", drink);
        return false; // Some weird error here...
    }
    const auto& ingredients = recipe->ingredients;

    // Nothing to do.
    if (ingredients.empty()) {
        Serial.println("ingredients empty");
        return false;
    }

    // Can't pour less than 1/8oz so 12 ingredients max.
    if (ingredients.size() > 8) {
        Serial.println("too many ingredients");
        return false;
    }

    // Calculate volumes - Short pours rather than overflow the cup.
    const uint8_t pourCount = floor(12 / ingredients.size());

    // Find ingredients!
    std::vector<std::pair<Controller, Pump>> pumps;
    for (const auto& ingredient: ingredients) {
        try {
            const auto liquor = liquorMap.at(ingredient);
            Serial.printf("found %s at 0x%x pump %d\n", ingredient.c_str(), liquor.first, liquor.second);
            pumps.push_back(liquor);
        } catch (const std::out_of_range&) {
            Serial.printf("Didn't find ingredient %s\n", ingredient.c_str());
            return false;
        }
    }

    // TODO: I want to optimize this and command all the pumps at once instead of sequentially but that seems like a lot of work right now.
    // TODO: Make pump controllers report back.  Wait on that.
    for (const auto& [controllerAddress, pump] : pumps) {
        Serial.printf("Requesting dispense from 0x%x, pump %d\n ", controllerAddress, pump);
        i2cBus->beginTransmission(controllerAddress);
            i2cBus->write(DISPENSE);
            i2cBus->write(pump);
            i2cBus->write(pourCount);
        if (const auto res = i2cBus->endTransmission(); res != 0) {
            // Error!  Abort!  Abort!
            Serial.printf("Error in i2c communication: %s.\n", (res == 1 ? "data too long" :
                                                                    res == 2 ? "NACK Received on address" :
                                                                    res == 3 ? "NACK received on data" :
                                                                    res == 5 ? "timeout" : "unknown error"));
// DEBUGGING: Continue on NACK            if (res == 2) continue;
            return false;
        }
//        delay(2320); // TODO: Use this if the pump controllers behave strangely.
    }
    return true;
}

std::optional<Recipe> RecipeBook::getRecipeAtIndex(int index) const {
    if (index >= recipes.size()) {
        Serial.println("Recipe index out of range");
        return {};
    }

    i2cBus->beginTransmission(DISPLAY_ADDRESS);
    i2cBus->write(index + 1);
    i2cBus->endTransmission();

    return { recipes[index] };
}
