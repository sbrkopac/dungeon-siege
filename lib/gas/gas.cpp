
/*
 * Copyright (c) 2016-2017 aaron andersen, sam brkopac
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "gas.hpp"

#include <fstream>
#include <sstream>
#include "gas.y.hpp"

namespace ehb
{
    static std::vector<std::string> split(const std::string & value, char delim)
    {
        std::vector<std::string> result;

        std::istringstream stream(value);

        while (!stream.eof())
        {
            std::string item;
            std::getline(stream, item, delim);

            result.push_back(item);
        }

        return result;
    }

    Node::~Node()
    {
        for (Node * node : mChildren)
        {
            delete node;
        }
    }

    Node * Node::appendChild(const std::string & name)
    {
        const auto index = name.find_last_of(':');

        if (index != std::string::npos)
        {
            const std::vector<std::string> path = split(name.substr(0, index), ':');

            Node * parent = this;

            for (const std::string & item : path)
            {
                Node * node = parent->child(item);

                if (!node)
                {
                    node = parent->appendChild(item);
                }

                parent = node;
            }

            return parent->appendChild(name.substr(index + 1));
        }
        else
        {
            // simply add a new child to this node with the given name
            Node * node = new Node(this);

            node->mName = name;

            mChildren.push_back(node);

            return node;
        }
    }

    Node * Node::appendChild(const std::string & name, const std::string & type)
    {
        Node * result = appendChild(name);

        result->mType = type;

        return result;
    }

    Node * Node::child(const std::string & name) const
    {
        // TODO: cleanup the code here

        std::vector<std::string> path = split(name, ':');

        switch (path.size())
        {
            case 1:
            {
                for (Node * node : mChildren)
                {
                    if (node->name() == name)
                    {
                        return node;
                    }
                }
            } break;

            default:
            {
                const Node * cItr = this;
                Node * finalAnswer = nullptr;

                for (const std::string & item : path)
                {
                    Node * result = nullptr;

                    for (Node * node : cItr->mChildren)
                    {
                        if (node->name() == item)
                        {
                            result = node;
                            break;
                        }
                    }

                    // did we find the node we're looking for?
                    if (result)
                    {
                        cItr = result;
                        finalAnswer = result;
                    }
                    else
                    {
                        return nullptr;
                    }
                }

                return finalAnswer;
            } break;
        }

        return nullptr;
    }

    std::vector<Node *> Node::eachChildOf(const std::string & name) const
    {
        std::vector<Node *> result;

        Node * node = this->child(name);

        if (node)
        {
            return node->mChildren;
        }

        return result;
    }

    std::vector<Attribute> Node::eachAttribute() const
    {
        return mAttributes;
    }

    void Node::appendValue(const std::string & name, const std::string & value)
    {
        const auto index = name.find_last_of(':');

        if (index != std::string::npos)
        {
            const std::vector<std::string> path = split(name.substr(0, index), ':');

            Node * parent = this;

            for (const std::string & item : path)
            {
                Node * node = parent->child(item);

                if (!node)
                {
                    node = parent->appendChild(item);
                }

                parent = node;
            }

            return parent->appendValue(name.substr(index + 1), value);
        }
        else
        {
            Attribute attr;

            attr.name = name;
            attr.value = value;

            mAttributes.push_back(attr);
        }
    }

    Node * Node::clone(Node * parent) const
    {
        Node * result = new Node(parent);

        result->mName = mName;
        result->mType = mType;

        for (const Node * child : mChildren)
        {
            result->mChildren.push_back(child->clone(result));
        }

        for (const Attribute & attr : mAttributes)
        {
            result->mAttributes.push_back(attr);
        }

        return result;
    }

    void Node::merge(Node * result) const
    {
        if (result)
        {
            result->mName = mName;
            result->mType = mType;

            if (isEmpty())
            {
                // clear out result!
                for (Node * child : result->mChildren)
                {
                    delete result;
                }

                result->mChildren.clear();
                result->mAttributes.clear();
            }
            else
            {
                for (const Node * i : mChildren)
                {
                    bool found = false;

                    for (Node * j : result->mChildren)
                    {
                        if (i->name() == j->name())
                        {
                            found = true;
                            i->merge(j);
                            break;
                        }
                    }

                    if (!found)
                    {
                        Node * copy = i->clone(result);
                        result->mChildren.push_back(copy);
                    }
                }

                for (const Attribute & i : mAttributes)
                {
                    bool found = false;

                    for (Attribute & j : result->mAttributes)
                    {
                        if (i.name == j.name)
                        {
                            found = true;
                            j = i;
                            break;
                        }
                    }

                    if (!found)
                    {
                        result->mAttributes.push_back(i);
                    }
                }
            }
        }
    }

    const Attribute * Node::attribute(const std::string & name) const
    {
        const auto index = name.find_last_of(':');

        const Node * parent;
        std::string actualName;

        if (index != std::string::npos)
        {
            parent = child(name.substr(0, index));
            actualName = name.substr(index + 1);
        }
        else
        {
            parent = this;
            actualName = name;
        }

        if (parent)
        {
            for (const Attribute & attr : parent->mAttributes)
            {
                if (attr.name == actualName)
                {
                    return &attr;
                }
            }
        }

        return nullptr;
    }

    bool Gas::load(std::istream & stream)
    {
        const std::string data(std::istreambuf_iterator<char>(stream), {});

        return gsparse_string(data.c_str(), this);
    }

    bool Gas::load(const std::string & filename)
    {
        return gsparse_file(filename.c_str(), this);
    }

    struct walkNode
    {
        walkNode(std::ostream & stream) : stream(stream), level(0)
        {
        }

        void write(const Node * node)
        {
            if (node->type() != "")
            {
                stream << indent() << "[t:" << node->type() << ",n:" << node->name() << "]" << std::endl;
            }
            else
            {
                stream << indent() << "[" << node->name() << "]" << std::endl;
            }

            stream << indent() << "{" << std::endl;

            level++;
            for (unsigned int i = 0; i < node->valueCount(); i++)
            {
                stream << indent() << node->nameOf(i) << " = " << node->valueOf(i) << ";" << std::endl;
            }

            for (Node * child : node->eachChild())
            {
                write(child);
            }
            level--;

            stream << indent() << "}" << std::endl;
        }

        inline std::string indent() const
        {
            return std::string(level * 4, ' ');
        }

        std::ostream & stream;
        unsigned int level;
    };

    bool Gas::save(std::ostream & stream) const
    {
        walkNode f(stream);

        for (Node * child : this->eachChild())
        {
            child->write(stream);
        }

        return true;
    }

    bool Gas::save(const std::string & filename) const
    {
        std::ofstream stream(filename);

        for (Node * child : this->eachChild())
        {
            child->write(stream);
        }

        stream.close();

        return true;
    }

    void Node::write(std::ostream & stream) const
    {
        walkNode f(stream);

        f.write(this);
    }
}
