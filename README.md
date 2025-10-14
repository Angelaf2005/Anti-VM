# Anti-VM
---
# Descripción del proyecto
Programa en C++ para detección de entornos virtuales. Detecta si un sistema se está ejecutando en una maquina virtual (VM) en Windows y Linux.

# Integrantes
    - Angel Adrian Alvares Flores               [CPUID]
    - Abraham Alejandro Carreon Soriano         [MacDetect]
    - Jesus Kenneth Maurizio Martinez Vazquez   [BiosVendor]
    - Raul Alejandro Rios Turrubiates           [Main]

# Tecnicas implementadas
- **CPUID (detención por bit de hipervisor y cadena del hypervisor)**  
  *Justificación:* Permite detectar virtualización a nivel de CPU leyendo el bit de hypervisor y la cadena identificadora del proveedor (p. ej. `VMwareVMware`, `VBoxVBoxVBox`). Es una técnica precisa y difícil de falsificar por el huésped sin alterar el hipervisor.

- **Lectura del BIOS Vendor (/sys/class/dmi/id/bios_vendor)**  
  *Justificación:* Muchos hipervisores exponen su identidad en los metadatos del firmware/BIOS. Leer y comparar el vendor es rápido, no requiere privilegios especiales y complementa la detección cuando el CPUID no está disponible o está ofuscado.

- **Detección por prefijos MAC (OUI) de interfaces de red**  
  *Justificación:* Las interfaces virtuales suelen usar rangos de MAC conocidos por fabricante (VMware, VirtualBox, etc.). Comparar el prefijo OUI ofrece una comprobación a nivel de red que detecta VMs incluso si se ocultan otras huellas.

# Compilación
## Windows
```
 g++ .\main.cpp .\macDetect.cpp .\WBiosVendor.cpp .\WCPUID.cpp -o anti-vm-windows.exe -liphlpapi
```
## Linux
```
g++ ./main.cpp ./macDetect.cpp ./LCPUID.cpp ./LBiosVendor.cpp -o anti-vm-linux -std=c++17
```

# Ejecución
## Windows
```
.\anti-vm-windows.exe
```
# Linux
```
./anti-vm-linux
```

# Tabla de resultados
## Linux
```
| Tecnica               | Win-Host | Win-VM (VBox) | Win-VM (VMware) | Lin-Host | Lin-VM (VBox) | Lin-VM (VMware) |
|-----------------------|----------|---------------|-----------------|----------|---------------|-----------------|
| CPUID Hypervisor Bit  | No       | No            | No              | No       | No            | Si              |
| BIOS Vendor String    | No       | No            | No              | Si       | No            | No              |
| MAC OUI               | No       | No            | No              | No       | No            | Si              |
```

## Windows
```
| Tecnica               | Win-Host | Win-VM (VBox) | Win-VM (VMware) | Lin-Host | Lin-VM (VBox) | Lin-VM (VMware) |
|-----------------------|----------|---------------|-----------------|----------|---------------|-----------------|
| CPUID Hypervisor Bit  | Si       | No            | No              | No       | No            | No              |
| BIOS Vendor String    | Si       | No            | No              | No       | No            | No              |
| MAC OUI               | Si       | No            | No              | No       | No            | No              |
```

# Conclusiones
- La detección por CPUID ofrece información fiable del hypervisor, pero algunos entornos virtuales pueden ocultarla.
- El método de BIOS Vendor es útil para identificar VMs de forma rápida, aunque puede ser falsificado o modificado.
- La verificación de MAC OUI complementa las otras técnicas y permite detectar interfaces virtuales, aunque puede generar falsos positivos en hardware flexible.
- La combinación de los tres métodos garantiza una mayor precisión en la identificación de máquinas virtuales, reduciendo el riesgo de falsos negativos.

# Licencia
Este proyecto se distribuye bajo la licencia **MIT**.
Consulta el archivo [LICENSE](./LICENSE) para mas información.