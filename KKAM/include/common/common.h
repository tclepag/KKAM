#pragma once

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "utils/console.h"
#include "utils/event.h"
#include "utils/string.h"
#include "math.h"

__declspec(align(16)) struct Transform {
	Matrix4 model;
	Matrix4 view;
	Matrix4 projection;
};

struct Vertex {
	float x, y, z;
	float r, g, b;
	float u, v;
};

template <typename T>
using Vector = std::vector<T>;
using CString = utils::String;
using String = std::wstring;
template <typename K, typename V>
using Map = std::unordered_map<K, V>;
using Thread = std::thread;
using Mutex = std::mutex;
using Lock = std::unique_lock<Mutex>;
using ConditionVariable = std::condition_variable;
template <typename T>
using Function = std::function<T>;
template <typename T>
using UniquePtr = std::unique_ptr<T>;
template <typename T>
using SharedPtr = std::shared_ptr<T>;
template <typename T>
using WeakPtr = std::weak_ptr<T>;
using InputFileStream = std::ifstream;
using OutputFileStream = std::ofstream;

inline String convertToWideString(const char* str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	String wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstrTo[0], size_needed);
	return wstrTo;
}