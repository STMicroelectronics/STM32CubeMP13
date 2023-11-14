# coding=utf-8
from cx_Freeze import setup, Executable
import os

if not os.path.exists('build/exe.win-amd64-2.7'):
   os.makedirs('build/exe.win-amd64-2.7')

base = None
# Remplacer "Stm32ImageAddHeader.py" par le nom du script qui lance votre programme
executables = [Executable("Stm32ImageAddHeader.py", base=base)]
# Renseignez ici la liste complète des packages utilisés par votre application
packages = ["struct", "time", "argparse"]
options = {
    'build_exe': {
        'build_exe': '../exe.win-amd64-2.7',
        'packages': packages,
    },
}
# Adaptez les valeurs des variables "name", "version", "description" à votre programme.
setup(
    name="ImageHeader",
    options=options,
    version="1.0",
    description='ImageHeader exe script',
    executables=executables
)