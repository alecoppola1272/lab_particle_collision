//OK
#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP
class ParticleType
{
public:
    ParticleType(const char *, double, int);
    const char *GetName() const;
    double GetMass() const;
    int GetCharge() const;
    virtual double GetWidth() const;
    virtual void Print() const;

private:
    const char *fName_;
    const double fMass_;
    const int fCharge_;
};

#endif