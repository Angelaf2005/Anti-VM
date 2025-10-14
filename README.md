# Anti-VM
---
# Descripción del proyecto
Programa en C++ para detección de entornos virtuales. Detecta si un sistema se está ejecutando en una maquina virtual (VM) en Windows y Linux.

# Integrantes
    - Angel Adrian Alvares Flores               []
    - Abraham Alejandro Carreon Soriano         []
    - Jesus Kenneth Maurizio Martinez Vazquez   []
    - Raul Alejandro Rios Turrubiates           []

# Compilación
## Windows
```
Codigo
```
## Linux
```
g++ ./main.cpp ./macDetect.cpp ./LCPUID.cpp ./LBiosVendor.cpp -o anti-vm-linux -std=c++17
```

# Ejecución
## Windows
```
Codigo
```
# Linux
```
./anti-vm-linux
```

# Tabla de resultados
```
| Tecnica               | Win-Host | Win-VM (VBox) | Win-VM (VMware) | Lin-Host | Lin-VM (VBox) | Lin-VM (VMware) |
|-----------------------|----------|---------------|-----------------|----------|---------------|-----------------|
| CPUID Hypervisor Bit  | No       | No            | No              | No       | No            | Si              |
| BIOS Vendor String    | No       | No            | No              | Si       | No            | No              |
| MAC OUI               | No       | No            | No              | No       | No            | Si              |
```

# Conclusiones