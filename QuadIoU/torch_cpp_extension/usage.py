# import torch
# import quad_iou
# import time

# a = torch.rand((200, 4, 2)).cuda() # gt. approx. dim
# b = torch.rand((300, 4, 2)).cuda() # pred. approx dim

# t1 = time.time()
# for i in range(600):
#     iou = quad_iou.calculateIoU(a, b)
# t2 = time.time()
# print(iou.shape)
# print(t2 - t1)

import numpy as np
import matplotlib.pyplot as plt

def generate_convex_quadrilateral():
    # Generate four random points
    points = np.random.rand(4, 2)

    # Calculate the centroid of the points
    centroid = np.mean(points, axis=0)

    # Calculate the angles of the points relative to the centroid
    angles = np.arctan2(points[:, 1] - centroid[1], points[:, 0] - centroid[0])

    # Sort the points by angle
    order = angles.argsort()
    points = points[order]

    # Return the sorted points
    return points

# Generate and draw 10 convex quadrilaterals
for i in range(10):
    # Generate a convex quadrilateral
    quadrilateral = generate_convex_quadrilateral()

    # Plot the convex quadrilateral
    plt.figure()
    plt.plot(*zip(*(np.vstack([quadrilateral, quadrilateral[0]]))), marker='o')
    plt.xlim(0, 1)
    plt.ylim(0, 1)
    plt.gca().set_aspect('equal', adjustable='box')

    # Save the figure to a file with a unique filename
    plt.savefig(f'convex_quadrilateral_{i+1}.png', bbox_inches='tight')
    plt.close()  # Close the figure to free up memory