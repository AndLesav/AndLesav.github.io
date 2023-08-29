# Création de la courbe E: y² = x^3 + ax + b sur Z/29Z
Zp = Zmod(29)
E = EllipticCurve(Zp, [-3, 5])

# Récupération des paramètres de cette courbe
a, b, Zp = E.a4(), E.a6(), E.base_field()

# Création d'un point sur la courbe
P = E((20,17)) # ou E.point((20,17))

# Vérifier si un élément est un carré dans Z/pZ
print(13, Zp(13).is_square()) # 13 est un carré modulo 29
print(2, Zp(2).is_square()) # 2 n'en est pas un

# Effectuer des opération sur un corps
x = 17 # x est défini sur les entiers
y = Zp(17) # y est défini sur Z/pZ
print(x/2) # Opération effectué sur les réels
print(y/2) # Opération effectué dans Z/pZ