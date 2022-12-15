from PIL import Image 

PATH = "C:/Users/takira/Desktop/ULB/MA2-IRCI/Q1/Virtual Reality/VR_Project/image/"
filename = "CubeMap_Boudiste.jpg"

img = Image.open(f"{PATH}{filename}") 
width, height = img.size

block_size = (width//4, height//3)

for i in range(4):
    for j in range(3):

        img_res = img.crop((i*block_size[0], (j)*block_size[1], (i+1)*block_size[0], (j+1)*block_size[1])) 
        if i==1 and j==0: # Top
            img_res.save(f"{PATH}posy.jpg")
        if i==0 and j==1: # Left
            img_res.save(f"{PATH}negx.jpg")
        if i==1 and j==1: # Center
            img_res.save(f"{PATH}posz.jpg")
        if i==2 and j==1: # Right
            img_res.save(f"{PATH}posx.jpg")
        if i==3 and j==1: # Right++
            img_res.save(f"{PATH}negz.jpg")
        if i==1 and j==2: # Bottom
            img_res.save(f"{PATH}negy.jpg")