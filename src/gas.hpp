
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

#ifndef GAS_HPP
#define GAS_HPP

#include <string>
#include <vector>

namespace ehb
{
    // private
    struct Attribute
    {
        std::string name;
        std::string type;
        std::string value;
    };

    // main element to make use of in this api
    class Node
    {
        public:

           ~Node();

            Node * parent() const;

            const std::string & name () const;
            const std::string & type () const;

            //! @return whether this node has no child nodes and attributes or not
            bool isEmpty() const;

            /**
             * @param name the full name of the new node to create
             * @param type the type of the new node to create
             * @return the newly created child node
             */
            Node * appendChild(const std::string & name);
            Node * appendChild(const std::string & name, const std::string & type);

            Node * child(const std::string & name) const;

            const std::vector<Node *> & eachChild() const;

            // TODO: return a const reference... decide what to do if the node path does not exist
            std::vector<Node *> eachChildOf(const std::string & name) const;

            std::vector<Attribute> eachAttribute() const;

            void appendValue(const std::string & name, const std::string & value);

            //! @return the number of attributes in this node
            unsigned int valueCount() const;

            /**
             * @param name which attribute type or value to return
             * @defaultValue the value to return if the attribute does not exist
             * @return the attribute type or value
             */
            const std::string & valueOf(const std::string & name, const std::string & defaultValue = "") const;
            const std::string & typeOf(const std::string & name, const std::string & defaultValue = "") const;

            /**
             * @param index which attribute name, type, or value to return ranging from 0 to valueCount()
             * @defaultValue the value to return if the attribute does not exist
             * @return the attribute name, type, or value
             */
            const std::string & nameOf(unsigned int index, const std::string & defaultValue = "") const;
            const std::string & typeOf(unsigned int index, const std::string & defaultValue = "") const;
            const std::string & valueOf(unsigned int index, const std::string & defaultValue = "") const;

            //! create a deep copy of the node that must be deleted by the user
            Node * clone(Node * parent = nullptr) const;

            /**
             * merge the contents of this node into the result node
             * NOTE: empty children from this node will entirely overwrite
             * (including all attributes and grand children nodes) children of
             * the same name in the result node
             */
            void merge(Node * result) const;

            void write(std::ostream & stream) const;

        protected:

            Node(Node * parent = nullptr);

        private:

            const Attribute * attribute(const std::string & name) const;

        private:

            std::string mName;
            std::string mType;
            Node * mParent;
            std::vector<Node *> mChildren;
            std::vector<Attribute> mAttributes;
    };

    inline Node::Node(Node * parent) : mParent(parent)
    {
    }

    inline Node * Node::parent() const
    {
        return mParent;
    }

    inline const std::string & Node::name () const
    {
        return mName;
    }

    inline const std::string & Node::type () const
    {
        return mType;
    }

    inline bool Node::isEmpty() const
    {
        return mChildren.empty() && mAttributes.empty();
    }

    inline const std::vector<Node *> & Node::eachChild() const
    {
        return mChildren;
    }

    inline unsigned int Node::valueCount() const
    {
        return mAttributes.size();
    }

    inline const std::string & Node::valueOf(const std::string & name, const std::string & defaultValue) const
    {
        const Attribute * attr = attribute(name);

        if (attr)
        {
            return attr->value;
        }

        return defaultValue;
    }

    inline const std::string & Node::typeOf(const std::string & name, const std::string & defaultValue) const
    {
        const Attribute * attr = attribute(name);

        if (attr)
        {
            return attr->type;
        }

        return defaultValue;
    }

    inline const std::string & Node::nameOf(unsigned int index, const std::string & defaultValue) const
    {
        if (index < mAttributes.size())
        {
            return mAttributes[index].name;
        }

        return defaultValue;
    }

    inline const std::string & Node::typeOf(unsigned int index, const std::string & defaultValue) const
    {
        if (index < mAttributes.size())
        {
            return mAttributes[index].type;
        }

        return defaultValue;
    }

    inline const std::string & Node::valueOf(unsigned int index, const std::string & defaultValue) const
    {
        if (index < mAttributes.size())
        {
            return mAttributes[index].value;
        }

        return defaultValue;
    }

    class Gas : public Node
    {
        public:

            bool load(std::istream & stream);
            bool load(const std::string & filename);

            bool save(std::ostream & stream) const;
            bool save(const std::string & filename) const;

    };
}

#endif // GAS_HPP
