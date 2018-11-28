import math
import random
import copy
import turtle

def observed(k):
    """read the observation points from txt
    for nearP, for AlphaBeta, For prediction
    """
    file = "Localization/CS585Bats-Localization_frame000000" + str(750+k) + ".txt"
    with open(file, "r") as ins:
        objects = []
        for line in ins:
            line = line.split(',')
            a = int(line[0])
            b = int(line[1])
            objects.append([a,b])
    return objects

def euclidean(a, b):
    """calculate euclidean between vector a and b
    """
    return math.sqrt((a[0]-b[0])**2+(a[1]-b[1])**2)

def color():
    r = random.randint(0, 255)
    g = random.randint(0, 255)
    b = random.randint(0, 255)
    rgb = [r, g, b]
    return rgb

def nearP(p,k):
    """calculate the nearest measurement to prediction p at frame k
    for AlphaBeta, for prediction
    """
    objectk = observed(k)
    min_d = math.inf
    m = objectk[0]
    for i in objectk:
        d = euclidean(i, p)
        if d < min_d:
            min_d = d
            m = i       
    return m

def AlphaBeta(k, p_k_1, v, A, B):
    """Apply AlphaBeta filter to p_k_1 to estimate prediction at k
    for prediction
    """
    p_k = []
    p_k.append(p_k_1[0] + v[0])
    p_k.append(p_k_1[1] + v[1])
    m = nearP(p_k, k)
    x = m[0] - p_k[0]
    y = m[1] - p_k[1]
    r = euclidean(m, p_k)
    p_k[0] = round(p_k[0] + A * x)
    p_k[1] = round(p_k[1] + A * y)
    v[0] = round(v[0] + B * r)
    v[1] = round(v[1] + B * r)
    #for i in p_k:
    #    i = i + A * r
    #for m in v:
    #   m = m + B * r
    return [v, p_k]
    

#init the track list
#contains the init velocity and measurements in the first frame
track = []
object0 = observed(0)
init_v = [0,0]
for i in object0:
    inst = []
    inst.append(init_v)
    c = color()
    inst.append(c)
    inst.append(i)
    track.append(inst)

#init current frame number k, alpha and beta
k = 1
A = 0.9
B = 0.005

def prediction(k, track):
    """calculate predictions at frame k
    """
    u = []
    for i in range(len(track)):
        c = track[i] #current bat
        v = c[0]
        p_k_1 = c[-1] #prediction at k-1
        print("p_k_1", p_k_1)
        update = AlphaBeta(k, p_k_1, v, A, B)
        print("update",update)
        u.append(update)
        c.append(update[1]) #add prediction at k to track
        c[0][0] = update[0][0] #update vx
        c[0][1] = update[0][1] #update vy
    return track
#print(prediction(k))

def find_index(a, j):
    """return the index in a that j appears
    for DataAssociation
    """
    for i in range(len(a)):
        if a[i] == j:
            index = i
            break
    return index

def nearest(m, i):
    """return nearest measurement in m to ith prediction, i is position vector
    for DataAssociation
    """
    i_d = math.inf
    near = m[0]
    for j in range(len(m)):
        d = euclidean(i, m[j])
        if d < i_d:
            i_d = d
            near = j
    return near

def kill(p, a, track):
    """return the bats that has prediction -1 and delete them from track
    for DataAssociation
    """
    negative_p = []
    for j in range(len(a)):
        if a[j] == -1:
            negative_p.append(j)
    for i in negative_p:
        trackdel = copy.deepcopy(track)
        item = track[i]
        trackdel.remove(item)
    #print(len(track))
    #print("track_killed", track)
    return trackdel

def add(m, a, track):
    """return the measurements that are not matched to any predictions and add them to the track
    for DataAssociation
    """
    empty_m = []
    empty_m_p = []
    for i in range(len(m)):
        if i not in a:
            empty_m.append(i)
            empty_m_p.append(m[i])
    #print("empty_m", empty_m)
    #print("empty_m_p", empty_m_p)
    for j in empty_m_p:
        new = []
        new.append(init_v)
        c = color()
        new.append(c)
        for k in range(len(track[0])-3):
            new.append([-1,-1])
        new.append(j)
        track.append(new)
    return track

def vectordiff(u, v):
    """calculate the difference between u and v
    for updateVelocity and DataAssociation
    """
    diff = []
    diff.append(u[0] - v[0])
    diff.append(u[1] - v[1])
    return diff

def updateVelocity(a, m, p, track):
    """update velocity for each old bats
    for DataAssociation
    """
    for i in range(len(a)):
        if a[i] != -1:
            track[i][0] = vectordiff(p[i], m[a[i]])
        else:
            continue
    #print("a", a)
    #print("m", m)
    #print("p", p)
    #print("track with velocity updated", track)
    return track

def DataAssociation(k, track):
    """Associate predictions with measurements at k
    """
    m = observed(k)
    p = []
    for i in range(len(track)):
        c = track[i]
        p.append(c[-1])
    """
    if len(m) < len(p): #bats are missing
        for i in range(len(p)-len(m)):
            m.append([-1,-1])
    elif len(m) > len(p): #new bats are showing
        for i in range(len(m)-len(p)):
            p.append([-1,-1])
    """
    #print("m:", m)
    #print("p:", p)
    a = [] #index is p, content is m
    for i in p: #i is current p
        near = nearest(m, i) #the index of nearest measurement near to i
        if near in a:
            index = find_index(a, near) #index of some other prediction has been assigned "near"
            if euclidean(i, m[near]) < euclidean(p[index], m[near]):
                a[index] = -1
            else:
                near = -1
        a.append(near)
    #print("a", a)
    track = updateVelocity(a, m, p, track)
    #print("velocityUpdated", track)
    track = kill(p, a, track)
    track = add(m, a, track)
    return track

k = 2
#for k in range(1, framenumber + 1):
track = prediction(k, track)
track = DataAssociation(k, track)

k = 3
track = prediction(k, track)
track = DataAssociation(k, track)
print('track', track)

    
