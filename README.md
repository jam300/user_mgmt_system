# 🧩 Task-Based User Management System

Este proyecto es una solución al ejercicio: un sistema de gestión de usuarios basado en tareas. Las operaciones se definen en archivos de texto y se ejecutan secuencialmente.

---

## 🛠️ Funcionalidades

- ✅ Crear, eliminar, deshabilitar usuarios
- ✅ Enviar mensajes y pings
- ✅ Gestionar grupos de usuarios
- ✅ Obtener historial de mensajes
- ✅ Sistema extensible para nuevos comandos

---

---

## 📄 Ejemplo de Entrada

Archivo `task1.txt`:

```
CREATE USER alice
SEND MESSAGE alice "Welcome!"
PING alice 2
GET USERS
EXIT
```

---

## 🧪 Pruebas Unitarias

```bash
cd build
ctest
```

---

## 🚀 Compilación (Modo Manual)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

> También puedes abrir el proyecto con Visual Studio directamente desde la carpeta `build`.

---

## 🔗 Dependencias

Este proyecto usa [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) para gestionar dependencias como:

- [`GoogleTest`](https://github.com/google/googletest): Framework de testing
- [`spdlog`](https://github.com/gabime/spdlog): Sistema de logging (opcional)

---

## 🧱 Requisitos

- CMake >= 3.20
- Compilador C++17 o superior (MSVC, Clang, GCC)
- Python 3 (solo requerido si usas scripts auxiliares)

---

## 📜 Autor

Desarrollado por **Javi Adán Méndez Méndez**.
