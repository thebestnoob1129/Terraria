#pragma once
#ifndef ENTITYIDHOLDER_H
#define ENTITYIDHOLDER_H

#include <cstdint>
#include <unordered_map>
#include <memory>

// Forward declaration to avoid circular include with entity.h
struct Entity;

constexpr static std::uint64_t PLAYER_ID = 1;

struct EntityIdHolder {
	std::uint64_t idCounter = 2;
	std::uint64_t getEntityIdAndIncrement();
};


struct EntityHolder {
	EntityIdHolder idHolder;
	std::unordered_map<std::uint64_t, std::unique_ptr<Entity>> entities;
};

#endif