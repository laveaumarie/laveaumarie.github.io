#!/bin/bash


# mkdir

mkdir -p $HOME/usr/bin


# mars

cp ./Mars4_5.jar $HOME/usr/bin/

echo '#!/bin/bash' > $HOME/usr/bin/mars.sh
echo 'java -jar $HOME/usr/bin/Mars4_5.jar' >> $HOME/usr/bin/mars.sh

chmod +x $HOME/usr/bin/mars.sh
