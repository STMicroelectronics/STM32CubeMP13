ImageHeader
-----------

ImageHeader allows to add a stm32 header needed by bootrom

Origin
^^^^^^

.. code:: bash

   $ git clone "ssh://frq09524@gerrit.st.com:29418/mpu/stm32cube/tools/imageheader"

Depending on your system, use the python27 or python3 repository

How to use
^^^^^^^^^^

- If you have elf file, the entry point and the load address can be exctrat from elf:

    .. code:: bash

       $ ./stm32_imgtool -e <elf file> -b <binary file> -o <output file> -bt <binary type>


- Else you must define these:

    .. code:: bash

       $ ./Stm32ImageAddHeader.py <binary file> <output file> -ep <entry_point> -la <load_addr> -bt <binary type>

- Binary type:

You find the different value of binry type in wiki `STM32_header_for_binary_files <https://wiki.st.com/stm32mpu/wiki/STM32_header_for_binary_files>`_
