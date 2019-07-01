#!/bin/bash

echo "begin"
docker run -it -d --name vm_hcc hcc
docker exec -it vm_hcc /bin/bash
