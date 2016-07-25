import os

if not os.path.exists('gl'):
    os.makedirs('gl')

with open('gl/gl_loader.h', 'wb') as f:
    f.write(br'''/*

    LINCENSE

*/''')
